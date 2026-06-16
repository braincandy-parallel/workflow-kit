# Network — IP Assignments

No external router. Sequencer ESP32 is the WiFi access point.

| Device | Role | IP | Notes |
|--------|------|----|-------|
| Sequencer ESP32 | AP gateway | 192.168.4.1 | AP SSID: `bison-ap` / Pass: `bisonaudio` |
| WLED ESP32 | Client | 192.168.4.11 | Set as static IP in WLED config → WiFi |
| Audio ESP32 | No WiFi | — | UART only (GPIO 16 RX from Sequencer GPIO 17 TX) |

## WLED static IP setup

In WLED web UI: Config → WiFi → Static IP: 192.168.4.11 / Gateway: 192.168.4.1 / Subnet: 255.255.255.0

## WLED preset trigger

Sequencer fires: `http://192.168.4.11/win&PL=1`
Change `PL=1` to match your preset number in WLED.
