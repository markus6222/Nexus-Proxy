/** 
 @file  win32.c
 @brief ENet Win32 system specific functions
*/
#ifdef _WIN32

#define ENET_BUILDING_LIB 1
#include "enet/enet.h"
#include <windows.h>
#include <mmsystem.h>
#include <ws2ipdef.h>
#include <stdint.h>
#include <stdlib.h>

/* ── SOCKS5 UDP tunnel globals ─────────────────────────────────────────────
   Set from C++ via socks5_tunnel::connect() before enet_host_connect().
   win32.c wraps/unwraps the SOCKS5 UDP relay header on every send/recv.   */
volatile int   g_socks5_active         = 0;
struct sockaddr_in g_socks5_relay_addr = {0};
SOCKET         g_socks5_control_socket = INVALID_SOCKET;

/* Build a 10-byte SOCKS5 UDP header (RSV RSV FRAG ATYP addr[4] port[2])
   into 'out' (caller must supply >=10 bytes).  Returns header length.    */
static int socks5_build_header(const struct sockaddr_in* target, unsigned char* out) {
    out[0] = 0x00; out[1] = 0x00; /* RSV */
    out[2] = 0x00;                 /* FRAG */
    out[3] = 0x01;                 /* ATYP IPv4 */
    memcpy(out + 4, &target->sin_addr, 4);
    memcpy(out + 8, &target->sin_port, 2);
    return 10;
}

static enet_uint32 timeBase = 0;

int
enet_initialize (void)
{
    WORD versionRequested = MAKEWORD (1, 1);
    WSADATA wsaData;
   
    if (WSAStartup (versionRequested, & wsaData))
       return -1;

    if (LOBYTE (wsaData.wVersion) != 1||
        HIBYTE (wsaData.wVersion) != 1)
    {
       WSACleanup ();
       
       return -1;
    }

    timeBeginPeriod (1);

    return 0;
}

void
enet_deinitialize (void)
{
    timeEndPeriod (1);

    WSACleanup ();
}

enet_uint32
enet_host_random_seed (void)
{
    return (enet_uint32) timeGetTime ();
}

enet_uint32
enet_time_get (void)
{
    return (enet_uint32) timeGetTime () - timeBase;
}

void
enet_time_set (enet_uint32 newTimeBase)
{
    timeBase = (enet_uint32) timeGetTime () - newTimeBase;
}

int
enet_address_set_host_ip (ENetAddress * address, const char * name)
{
    enet_uint8 vals [4] = { 0, 0, 0, 0 };
    int i;

    for (i = 0; i < 4; ++ i)
    {
        const char * next = name + 1;
        if (* name != '0')
        {
            long val = strtol (name, (char **) & next, 10);
            if (val < 0 || val > 255 || next == name || next - name > 3)
              return -1;
            vals [i] = (enet_uint8) val;
        }

        if (* next != (i < 3 ? '.' : '\0'))
          return -1;
        name = next + 1;
    }

    memcpy (& address -> host, vals, sizeof (enet_uint32));
    return 0;
}

int
enet_address_set_host (ENetAddress * address, const char * name)
{
    struct hostent * hostEntry;

    hostEntry = gethostbyname (name);
    if (hostEntry == NULL ||
        hostEntry -> h_addrtype != AF_INET)
      return enet_address_set_host_ip (address, name);

    address -> host = * (enet_uint32 *) hostEntry -> h_addr_list [0];

    return 0;
}

int
enet_address_get_host_ip (const ENetAddress * address, char * name, size_t nameLength)
{
    char * addr = inet_ntoa (* (struct in_addr *) & address -> host);
    if (addr == NULL)
        return -1;
    else
    {
        size_t addrLen = strlen(addr);
        if (addrLen >= nameLength)
          return -1;
        memcpy (name, addr, addrLen + 1);
    }
    return 0;
}

int
enet_address_get_host (const ENetAddress * address, char * name, size_t nameLength)
{
    struct in_addr in;
    struct hostent * hostEntry;
 
    in.s_addr = address -> host;
    
    hostEntry = gethostbyaddr ((char *) & in, sizeof (struct in_addr), AF_INET);
    if (hostEntry == NULL)
      return enet_address_get_host_ip (address, name, nameLength);
    else
    {
       size_t hostLen = strlen (hostEntry -> h_name);
       if (hostLen >= nameLength)
         return -1;
       memcpy (name, hostEntry -> h_name, hostLen + 1);
    }

    return 0;
}

int
enet_socket_bind (ENetSocket socket, const ENetAddress * address)
{
    struct sockaddr_in sin;

    memset (& sin, 0, sizeof (struct sockaddr_in));

    sin.sin_family = AF_INET;

    if (address != NULL)
    {
       sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
       sin.sin_addr.s_addr = address -> host;
    }
    else
    {
       sin.sin_port = 0;
       sin.sin_addr.s_addr = INADDR_ANY;
    }

    return bind (socket,
                 (struct sockaddr *) & sin,
                 sizeof (struct sockaddr_in)) == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_get_address (ENetSocket socket, ENetAddress * address)
{
    struct sockaddr_in sin;
    int sinLength = sizeof (struct sockaddr_in);

    if (getsockname (socket, (struct sockaddr *) & sin, & sinLength) == -1)
      return -1;

    address -> host = (enet_uint32) sin.sin_addr.s_addr;
    address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);

    return 0;
}

int
enet_socket_listen (ENetSocket socket, int backlog)
{
    return listen (socket, backlog < 0 ? SOMAXCONN : backlog) == SOCKET_ERROR ? -1 : 0;
}

ENetSocket
enet_socket_create (ENetSocketType type)
{
    return socket (PF_INET, type == ENET_SOCKET_TYPE_DATAGRAM ? SOCK_DGRAM : SOCK_STREAM, 0);
}

int
enet_socket_set_option (ENetSocket socket, ENetSocketOption option, int value)
{
    int result = SOCKET_ERROR;
    switch (option)
    {
        case ENET_SOCKOPT_NONBLOCK:
        {
            u_long nonBlocking = (u_long) value;
            result = ioctlsocket (socket, FIONBIO, & nonBlocking);
            break;
        }

        case ENET_SOCKOPT_BROADCAST:
            result = setsockopt (socket, SOL_SOCKET, SO_BROADCAST, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_REUSEADDR:
            result = setsockopt (socket, SOL_SOCKET, SO_REUSEADDR, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_RCVBUF:
            result = setsockopt (socket, SOL_SOCKET, SO_RCVBUF, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_SNDBUF:
            result = setsockopt (socket, SOL_SOCKET, SO_SNDBUF, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_RCVTIMEO:
            result = setsockopt (socket, SOL_SOCKET, SO_RCVTIMEO, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_SNDTIMEO:
            result = setsockopt (socket, SOL_SOCKET, SO_SNDTIMEO, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_NODELAY:
            result = setsockopt (socket, IPPROTO_TCP, TCP_NODELAY, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_TTL:
            result = setsockopt (socket, IPPROTO_IP, IP_TTL, (char *) & value, sizeof (int));
            break;

        default:
            break;
    }
    return result == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_get_option (ENetSocket socket, ENetSocketOption option, int * value)
{
    int result = SOCKET_ERROR, len;
    switch (option)
    {
        case ENET_SOCKOPT_ERROR:
            len = sizeof(int);
            result = getsockopt (socket, SOL_SOCKET, SO_ERROR, (char *) value, & len);
            break;

        case ENET_SOCKOPT_TTL:
            len = sizeof(int);
            result = getsockopt (socket, IPPROTO_IP, IP_TTL, (char *) value, & len);
            break;

        default:
            break;
    }
    return result == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_connect (ENetSocket socket, const ENetAddress * address)
{
    struct sockaddr_in sin;
    int result;

    memset (& sin, 0, sizeof (struct sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
    sin.sin_addr.s_addr = address -> host;

    result = connect (socket, (struct sockaddr *) & sin, sizeof (struct sockaddr_in));
    if (result == SOCKET_ERROR && WSAGetLastError () != WSAEWOULDBLOCK)
      return -1;

    return 0;
}

ENetSocket
enet_socket_accept (ENetSocket socket, ENetAddress * address)
{
    SOCKET result;
    struct sockaddr_in sin;
    int sinLength = sizeof (struct sockaddr_in);

    result = accept (socket, 
                     address != NULL ? (struct sockaddr *) & sin : NULL, 
                     address != NULL ? & sinLength : NULL);

    if (result == INVALID_SOCKET)
      return ENET_SOCKET_NULL;

    if (address != NULL)
    {
        address -> host = (enet_uint32) sin.sin_addr.s_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);
    }

    return result;
}

int
enet_socket_shutdown (ENetSocket socket, ENetSocketShutdown how)
{
    return shutdown (socket, (int) how) == SOCKET_ERROR ? -1 : 0;
}

void
enet_socket_destroy (ENetSocket socket)
{
    if (socket != INVALID_SOCKET)
      closesocket (socket);
}

int
enet_socket_send (ENetSocket socket,
                  const ENetAddress * address,
                  const ENetBuffer * buffers,
                  size_t bufferCount)
{
    struct sockaddr_in sin;
    DWORD sentLength = 0;

    if (address != NULL)
    {
        memset (& sin, 0, sizeof (struct sockaddr_in));
        sin.sin_family = AF_INET;
        sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
        sin.sin_addr.s_addr = address -> host;
    }

    /* ── SOCKS5 path: prepend relay header, send to relay addr ─────────── */
    if (g_socks5_active && address != NULL)
    {
        unsigned char hdr[10];
        int hdr_len = socks5_build_header(&sin, hdr);

        /* Flatten all ENet buffers + header into one allocation */
        size_t total = (size_t)hdr_len;
        size_t i;
        for (i = 0; i < bufferCount; ++i)
            total += buffers[i].dataLength;

        unsigned char* pkt = (unsigned char*)malloc(total);
        if (!pkt) return -1;

        memcpy(pkt, hdr, hdr_len);
        size_t off = hdr_len;
        for (i = 0; i < bufferCount; ++i) {
            memcpy(pkt + off, buffers[i].data, buffers[i].dataLength);
            off += buffers[i].dataLength;
        }

        int result = sendto(socket,
                            (const char*)pkt, (int)total, 0,
                            (struct sockaddr*)&g_socks5_relay_addr,
                            sizeof(struct sockaddr_in));
        free(pkt);

        if (result == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) return 0;
            return -1;
        }
        /* Report original payload length (minus the header we added) */
        return result > hdr_len ? result - hdr_len : 0;
    }

    /* ── Normal (non-SOCKS5) path ────────────────────────────────────────── */
    if (WSASendTo (socket,
                   (LPWSABUF) buffers,
                   (DWORD) bufferCount,
                   & sentLength,
                   0,
                   address != NULL ? (struct sockaddr *) & sin : NULL,
                   address != NULL ? sizeof (struct sockaddr_in) : 0,
                   NULL,
                   NULL) == SOCKET_ERROR)
    {
       if (WSAGetLastError () == WSAEWOULDBLOCK)
         return 0;
       return -1;
    }

    return (int) sentLength;
}

int
enet_socket_receive (ENetSocket socket,
                     ENetAddress * address,
                     ENetBuffer * buffers,
                     size_t bufferCount)
{
    INT sinLength = sizeof (struct sockaddr_in);
    DWORD flags = 0, recvLength = 0;
    struct sockaddr_in sin;

    /* ── SOCKS5 path: strip relay header, extract real source ───────────── */
    if (g_socks5_active)
    {
        /* Receive into first buffer — ENet always passes exactly 1 buffer here */
        recvLength = 0;
        flags      = 0;
        sinLength  = sizeof(struct sockaddr_in);

        if (WSARecvFrom(socket,
                        (LPWSABUF)buffers,
                        (DWORD)bufferCount,
                        &recvLength,
                        &flags,
                        (struct sockaddr*)&sin,
                        &sinLength,
                        NULL, NULL) == SOCKET_ERROR)
        {
            switch (WSAGetLastError()) {
            case WSAEWOULDBLOCK:
            case WSAECONNRESET:
                return 0;
            }
            return -1;
        }

        if (flags & MSG_PARTIAL) return -1;
        if (recvLength < 10)     return 0;  /* too short to have a SOCKS5 header */

        /* Parse the relay header: RSV(2) FRAG(1) ATYP(1) addr(4) port(2) = 10 bytes */
        unsigned char* raw = (unsigned char*)buffers[0].data;
        if (raw[3] != 0x01) return 0;  /* only IPv4 */

        /* Real source address */
        if (address != NULL) {
            memcpy(&address->host, raw + 4, 4);
            uint16_t nport;
            memcpy(&nport, raw + 8, 2);
            address->port = ENET_NET_TO_HOST_16(nport);
        }

        /* Shift payload over the header in-place */
        int payload_len = (int)recvLength - 10;
        if (payload_len <= 0) return 0;
        memmove(raw, raw + 10, payload_len);
        return payload_len;
    }

    /* ── Normal (non-SOCKS5) path ────────────────────────────────────────── */
    if (WSARecvFrom (socket,
                     (LPWSABUF) buffers,
                     (DWORD) bufferCount,
                     & recvLength,
                     & flags,
                     address != NULL ? (struct sockaddr *) & sin : NULL,
                     address != NULL ? & sinLength : NULL,
                     NULL,
                     NULL) == SOCKET_ERROR)
    {
       switch (WSAGetLastError ())
       {
       case WSAEWOULDBLOCK:
       case WSAECONNRESET:
          return 0;
       }
       return -1;
    }

    if (flags & MSG_PARTIAL)
      return -1;

    if (address != NULL)
    {
        address -> host = (enet_uint32) sin.sin_addr.s_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);
    }

    return (int) recvLength;
}

int
enet_socketset_select (ENetSocket maxSocket, ENetSocketSet * readSet, ENetSocketSet * writeSet, enet_uint32 timeout)
{
    struct timeval timeVal;

    timeVal.tv_sec = timeout / 1000;
    timeVal.tv_usec = (timeout % 1000) * 1000;

    return select (maxSocket + 1, readSet, writeSet, NULL, & timeVal);
}

int
enet_socket_wait (ENetSocket socket, enet_uint32 * condition, enet_uint32 timeout)
{
    fd_set readSet, writeSet;
    struct timeval timeVal;
    int selectCount;
    
    timeVal.tv_sec = timeout / 1000;
    timeVal.tv_usec = (timeout % 1000) * 1000;
    
    FD_ZERO (& readSet);
    FD_ZERO (& writeSet);

    if (* condition & ENET_SOCKET_WAIT_SEND)
      FD_SET (socket, & writeSet);

    if (* condition & ENET_SOCKET_WAIT_RECEIVE)
      FD_SET (socket, & readSet);

    selectCount = select (socket + 1, & readSet, & writeSet, NULL, & timeVal);

    if (selectCount < 0)
      return -1;

    * condition = ENET_SOCKET_WAIT_NONE;

    if (selectCount == 0)
      return 0;

    if (FD_ISSET (socket, & writeSet))
      * condition |= ENET_SOCKET_WAIT_SEND;
    
    if (FD_ISSET (socket, & readSet))
      * condition |= ENET_SOCKET_WAIT_RECEIVE;

    return 0;
} 

#endif

