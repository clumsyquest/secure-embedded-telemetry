# J2 — Security layer: AES-GCM authenticated telemetry

## System
ESP32-S3 sensor node -> WiFi/UDP -> Raspberry Pi gateway. Payload: JSON telemetry.

## Attacker (on the local WiFi)
Can sniff, tamper, spoof/inject, and replay packets.

## Goals & mechanisms
- Confidentiality -> AES-256-GCM encryption.
- Integrity & authenticity -> GCM 16-byte tag, verified on the gateway; tampered/forged packets are rejected.
- Anti-replay -> 64-bit monotonic counter used as the GCM nonce (unique per message).

## Crypto parameters
- AES-256-GCM (AEAD). Nonce: 12 bytes from a 64-bit message counter. Tag: 16 bytes. No AAD.
- Wire format: nonce(12) || ciphertext(N) || tag(16).

## Key management
- 256-bit pre-shared symmetric key, identical on node and gateway (hardcoded placeholder).
- Out of scope for J2; production would provision per-device keys via a Secure Element (ATECC608).

## Out of scope
Key distribution, physical attacks, side-channels, denial of service.

## Status
- [x] Encryption + authentication working end-to-end (on-device self-test AND live ESP32 -> Pi).
- [ ] Anti-replay enforcement on the gateway (reject nonce <= last seen) — TODO.
- [ ] Real key provisioning (Secure Element) — TODO.
