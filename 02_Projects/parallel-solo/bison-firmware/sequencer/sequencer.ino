// Bison Installation — Sequencer ESP32
// Board: ESP32-WROOM-32E
//
// Responsibilities:
//   - Read TTP223 touch inputs (snout + horns)
//   - Enforce retrigger lock for duration of audio file
//   - Send UART byte to Audio ESP32
//   - Send HTTP GET to WLED ESP32 to trigger lighting preset
//   - Act as WiFi AP for WLED

#include <WiFi.h>
#include <HTTPClient.h>

// ── Network ───────────────────────────────────────────────────
const char*       AP_SSID     = "bison-ap";
const char*       AP_PASS     = "bisonaudio";
const IPAddress   AP_IP(192, 168, 4, 1);
const IPAddress   AP_GATEWAY(192, 168, 4, 1);
const IPAddress   AP_SUBNET(255, 255, 255, 0);
const IPAddress   WLED_IP(192, 168, 4, 11);
const int         WLED_PRESET = 1;

// ── Pins ──────────────────────────────────────────────────────
const int PIN_TOUCH = 4;    // TTP223 output → GPIO 4
                             // TTP223 pad input wired to snout + both horn electrodes
                             // (all zones electrically unified — one sensor, one GPIO)
const int UART_TX   = 17;   // UART2 TX → Audio ESP32 GPIO 16
const int UART_RX   = 16;   // UART2 RX (not used; must declare for Serial2.begin)

// ── Timing ────────────────────────────────────────────────────
// UPDATE this constant when JB delivers final audio file.
// Value = exact audio file duration in milliseconds.
const unsigned long AUDIO_DURATION_MS = 30000;
const unsigned long DEBOUNCE_MS       = 50;

// ── State ─────────────────────────────────────────────────────
bool          locked        = false;
unsigned long lockStartMs   = 0;
unsigned long touchOnsetMs  = 0;

// ── Functions ─────────────────────────────────────────────────
bool touchActive() {
  return digitalRead(PIN_TOUCH) == HIGH;
}

void triggerAudio() {
  Serial2.write('T');
}

void triggerWLED() {
  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("[WLED] no client connected — skipping");
    return;
  }
  HTTPClient http;
  String url = "http://" + WLED_IP.toString() + "/win&PL=" + String(WLED_PRESET);
  http.begin(url);
  int code = http.GET();
  http.end();
  Serial.printf("[WLED] HTTP GET %s → %d\n", url.c_str(), code);
}

void startSequence() {
  locked      = true;
  lockStartMs = millis();
  Serial.println("[SEQ] Sequence start — lock ON");
  triggerAudio();   // UART first — zero latency
  triggerWLED();    // HTTP next — ~20ms, imperceptible offset
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(PIN_TOUCH, INPUT);

  Serial2.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  Serial.println("[UART] Serial2 ready");

  WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("[WiFi] AP: %s  IP: %s\n", AP_SSID, WiFi.softAPIP().toString().c_str());
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  // Release lock after audio duration
  if (locked && (millis() - lockStartMs >= AUDIO_DURATION_MS)) {
    locked = false;
    Serial.println("[SEQ] Lock cleared — idle");
  }

  if (locked) return;

  // Debounced touch detection
  if (touchActive()) {
    if (touchOnsetMs == 0) touchOnsetMs = millis();
    if (millis() - touchOnsetMs >= DEBOUNCE_MS) {
      touchOnsetMs = 0;
      startSequence();
    }
  } else {
    touchOnsetMs = 0;
  }
}
