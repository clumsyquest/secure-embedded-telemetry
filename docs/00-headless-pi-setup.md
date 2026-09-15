# Milestone 0 — Headless Linux gateway online

**Goal:** flash a Raspberry Pi Zero 2 W and reach it over SSH with **no screen and no keyboard** (headless), so it can serve as the project's Linux gateway.

**Result:** ✅ done — logged in over SSH via public-key authentication.

---

## What "headless" means here

The Pi runs with no monitor. It joins a Wi-Fi network on its own at boot, gets an IP address, and I connect to it from my laptop over SSH. All configuration is baked onto the SD card *before* first boot.

## Steps

1. **Flash Raspberry Pi OS Lite** onto the microSD with Raspberry Pi Imager, using the advanced options (⚙️) to preset:
   - hostname (`tth`) and user (`clumsyquest`);
   - **SSH enabled, public-key authentication only** (my public key pasted in);
   - Wi-Fi network (SSID + password).
2. Insert the card, power the Pi through the **`PWR IN`** micro-USB port.
3. Wait ~1–2 min for the first boot (filesystem resize + Wi-Fi join).
4. Find the Pi's IP and connect:
   ```
   ssh clumsyquest@<pi-ip>
   ```
5. Verify:
   ```
   whoami        # clumsyquest
   uname -a      # Linux tth ... aarch64  → 64-bit ARM
   ```

## Hurdles I hit (and how I solved them) — the real part

- **The SD card wasn't visible in WSL.** WSL2 is a VM and doesn't see USB storage by default; `usbipd` + an SD-card reader is unreliable. **Fix:** flashed from native Linux / the Windows Imager instead — flashing doesn't need WSL.
- **The Pi wouldn't join my phone hotspot.** The Pi Zero 2 W is **2.4 GHz only**; an iPhone hotspot defaults to 5 GHz. **Fix:** enabled *Maximise Compatibility* on the iPhone (forces 2.4 GHz), then re-plugged the Pi.
- **My residence Wi-Fi has a captive portal**, which a headless device can't log into. **Fix:** used the phone hotspot as the network instead.
- **`.local` name didn't resolve from WSL.** WSL2 doesn't forward mDNS. **Fix:** connected by IP address (iPhone hotspot subnet is `172.20.10.x`).
- **`Permission denied (publickey)`** — the key on the Pi didn't match the private key I was using. **Fix:** made sure the public key placed on the Pi was the pair of the private key in `~/.ssh/`, with correct permissions (`chmod 600`).
- **`REMOTE HOST IDENTIFICATION HAS CHANGED`** after re-flashing — the Pi's host key changed. **Fix:** removed the stale entry with `ssh-keygen -R <ip>` and re-accepted the new one.

## Security note

SSH is configured for **public-key authentication** from the start — no password login. This is the first hardening decision of the project: passwords can be brute-forced (the root cause of the Mirai IoT botnet), keys cannot. Disabling password login entirely is planned for Milestone 4.

## Next

Milestone 1 — get the first telemetry flowing: an ESP32 reading a sensor and sending it to this gateway over Wi-Fi/UDP (plaintext first, encryption comes in M2).
