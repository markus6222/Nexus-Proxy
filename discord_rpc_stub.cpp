// Discord rich presence is cosmetic here -- it only sets the "playing" card on a
// Discord profile. The prebuilt discord-rpc.lib was not part of this source drop,
// so these no-ops satisfy the link and the proxy runs without it.
// To restore real presence: build discord-rpc, drop the .lib back into the project,
// and remove this file.
#include "discord_rpc.h"

extern "C" {

void Discord_Initialize(const char* /*applicationId*/,
                        DiscordEventHandlers* /*handlers*/,
                        int /*autoRegister*/,
                        const char* /*optionalSteamId*/) {}

void Discord_Shutdown(void) {}
void Discord_RunCallbacks(void) {}
void Discord_UpdateConnection(void) {}
void Discord_UpdatePresence(const DiscordRichPresence* /*presence*/) {}
void Discord_ClearPresence(void) {}
void Discord_Respond(const char* /*userid*/, int /*reply*/) {}
void Discord_UpdateHandlers(DiscordEventHandlers* /*handlers*/) {}

} // extern "C"
