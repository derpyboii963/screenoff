# screenoff

Tesla overlay: fullscreen black layer (OLED pixels off) that swallows all input
until you hold L+R+A. Meant for long game/installer transfers (DBI, Cyberfoil).

Status: UNTESTED on hardware. API calls were checked against the bundled libtesla
headers (WerWolv/libtesla @ f766e9b), but it has not been compiled or run.

## Build
1. Install devkitPro + `switch-dev` (libnx). `export DEVKITPRO=/opt/devkitpro`
2. `make`  ->  `screenoff.ovl`
3. Copy `screenoff.ovl` to `sd:/switch/.overlays/`

## Use
Open Tesla, pick ScreenOff, press A on "Screen off". Wake: hold L+R+A.
Also un-blacks: Tesla launch combo, HOME, power button (libtesla hides the overlay).
Change the wake combo via WAKE_COMBO in source/main.cpp.

## Build without installing anything
Push this folder's contents as the root of a GitHub repo. The workflow in
`.github/workflows/build.yml` builds in the `devkitpro/devkita64` container;
download `screenoff.ovl` from the run's Artifacts. (Workflow not yet run.)
Or with Docker: `docker run --rm -v "$PWD":/w -w /w devkitpro/devkita64 make`
