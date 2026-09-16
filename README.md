# Nexus Premium Proxy

Windows ENet proxy for **Growtopia**. The client talks to `127.0.0.1:17191`; the proxy speaks to Ubisoft’s game servers. Chat commands (`/proxy` in-game) drive movement, casino hosting, autocollect, pathfinding, and visual extras.

Current title string: **Nexus Premium Proxy V1.1**.

## How it works

1. Run `Nexus Proxy.exe` **as Administrator** (the project sets UAC to require it).
2. On start it fetches `https://www.growtopia1.com/growtopia/server_data.php`, then writes `hosts`:

   ```
   127.0.0.1 www.growtopia1.com
   127.0.0.1 www.growtopia2.com
   ```

   and flushes DNS.
3. Listen port **17191**. Growtopia is redirected to the local proxy; the proxy forwards to the real `server:port` from `server_data`.
4. Data and logs go under `C:\Users\Public\Nexus_Stuff\` (`save`, `save\clothes`, `Logs`).
5. Closing the proxy is supposed to clear `hosts`. If it crashes, remove the two `127.0.0.1 www.growtopia*.com` lines yourself or the game will keep hitting localhost.

*A previous crash can leave those hosts lines in place. Startup truncates `hosts` before the HTTP fetch so it does not resolve Ubisoft to itself.*

## Features

In Growtopia chat: **`/proxy`** for the command dialog.

### Casino / hosting
`/pos` `/pos1` `/pos2` `/tp` `/win1` `/w1` `/win2` `/w2` `/reset` `/gems` `/track` `/logs` `/instantgem` `/ig` `/numbername` `/nn` `/fastdoor` `/spam` `//` `/realspin` `/spinqq` `/spinreme` `/scan` `/tax` `/game` `/gdrop` `/gd`

### Movement / world
`/antigravity` `/antighost` `/antizombie` `/ghost` `/pf` (pathfinder, Shift+Punch) `/teleport [name]` `/pathfinder` `/speed` `/save` `/setsave [world]` `/back` `/warp [world]` `/relog` `/res` `/fps`

### Inventory / drop
`/fd` `/ft` `/daw` `/dropall` `/cd` `/cdrop` `/dd` `/ddrop` `/split [%]` `/inventory` `/balance` `/find` `/vdrop [id]`

### Automation
`/collect` `/ac` `/autocollect` `/autosurg` `/autocrime` `/autofarm` `/farm` `/automsg` `/setmsg` `/autowrench` `/autopull` `/autoban` `/fireban` `/autoacc` `/gp` `/emptybox` `/dcount` `/fastempty` `/vend`

### Visual / account
`/weather` `/title` `/clothes` `/vc` `/mod` `/blink` `/name [name]` `/flag [id]` `/account` `/wrench` `/fakeban` `/settings` `/options` `/hotkeys` `/gscan` `/growscan`

### Moderation-style
`/pullall` `/kickall` `/banall`

Hotkeys: `/hotkeys`. World parse feeds `/scan`, `/growscan`, `/pathfinder`.

## Build

### Requirements
| Piece | Notes |
| --- | --- |
| Visual Studio 2022 | Desktop C++, toolset **v143** |
| OpenSSL | Bundled under `third_party/openssl` (headers + `lib\x64\MT` / `MTd`). No system install. |
| Windows x64 | **Release \| x64** is the shipping config. |

### Steps
1. Clone this repo.
2. Open **`proxy.vcxproj` at the repo root**. Configuration **Release**, platform **x64**.  
   *`proxy.sln` still references `proxy\proxy.vcxproj` — that nested path is wrong. Use the vcxproj.*
3. Build.
4. Output: **`x64\Release\Nexus Proxy.exe`**.

### If link fails
- **unresolved libssl / libcrypto** — the project expects the bundled **MT** x64 static libs under `third_party/openssl`, not a system OpenSSL MD/Win32 tree.

## License

OpenSSL: `third_party/openssl/LICENSE.txt`.
