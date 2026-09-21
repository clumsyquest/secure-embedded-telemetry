# Secure Embedded Telemetry

A bare-metal sensor node that sends **encrypted, authenticated** telemetry to a **hardened embedded-Linux gateway**.

Built in public as I learn embedded Linux and embedded security — from bare-metal microcontrollers to the Linux kernel. Real bugs, real fixes, nothing hidden.

> **Status:** 🚧 Work in progress · **Milestone 0 complete** (headless gateway online).

---

## Why this project

Nearly every connected product now needs *security by design* — encryption, a minimal attack surface, and proper vulnerability handling (see the EU Cyber Resilience Act, mandatory from Dec 2027). This project builds exactly that, end to end, on real hardware:

- a **constrained sensor node** (bare-metal) that trusts nothing and encrypts everything it sends;
- a **Linux gateway** that verifies, logs, and is progressively hardened;
- a written **threat model** and a documented hardening pass.

It is both a learning project and a portfolio piece for embedded / real-time / security roles.

---

## Architecture

```
   [ Sensor ]                              [ Raspberry Pi Zero 2 W — Linux ]
       |                                              |
   [ ESP32 ]  --- WiFi / UDP (AES encrypted) --->  [ gateway service ]
   read + encrypt                                   decrypt -> verify -> log
   + replay counter                                 -> dashboard
```

**Security principle:** the node trusts nothing; the gateway verifies everything. Every message is encrypted (AES) and numbered (counter) to defeat replay attacks.

---

## Roadmap

| # | Milestone | Status |
|---|---|---|
| 0 | **Headless Linux gateway online** (flash, Wi-Fi, SSH key auth) | ✅ Done |
| 1 | Telemetry link — ESP32 → Wi-Fi/UDP → Pi (plaintext first) | ✅ Done |
| 2 | Security layer — AES payload + anti-replay counter + threat model | ⬜ |
| 3 | Gateway as a real service — systemd, logging, small dashboard | ⬜ |
| 4 | Hardening — read-only rootfs, firewall, key-only SSH, write-up | ⬜ |
| 5 | Custom **Buildroot** image that boots and runs the service | ⬜ |
| 6 | Sub-GHz radio transport (STM32 + SPIRIT1) as an alternative link | ⬜ |

Full milestone details: [`docs/`](docs/).

---

## Hardware

- **Gateway:** Raspberry Pi Zero 2 W (ARM Cortex-A53, aarch64) + microSD
- **Node:** ESP32 (Wi-Fi) — STM32 + SPIRIT1 sub-GHz radio for the later transport
- Sensors, multimeter, soldering iron

---

## Repository layout

```
.
├── README.md
├── docs/
│   └── 00-headless-pi-setup.md   # Milestone 0 — what I did and the hurdles I solved
├── node/                         # sensor-node firmware (ESP32) — coming in M1
├── gateway/                      # gateway service (Pi) — coming in M1
└── .gitignore
```

---

## Progress log

- **M0 — done.** Raspberry Pi Zero 2 W set up as a headless gateway: Raspberry Pi OS flashed, Wi-Fi + SSH **public-key** auth configured at flash time, reachable over the network with no screen or keyboard. Notes and troubleshooting in [`docs/00-headless-pi-setup.md`](docs/00-headless-pi-setup.md).

---

## About

I'm an embedded-systems engineering student (EHTP – Centrale Nantes) building toward embedded security. I document this project as I go.

- GitHub: [github.com/clumsyquest](https://github.com/clumsyquest)
