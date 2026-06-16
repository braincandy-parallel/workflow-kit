// Bison Installation — Bench Test Sequencer
// Replaces the production sequencer for home bench testing.
//
// DIFFERENCES FROM PRODUCTION (sequencer.ino):
//   - Joins home WiFi (STA mode) instead of acting as AP
//   - Button on GPIO 4 with INPUT_PULLUP (LOW = pressed)
//     vs TTP223 (HIGH = touched) in production
//   - No UART TX (audio ESP32 not in this bench setup)
//   - WLED_IP points to home network address of your WLED ESP32
//
// Wiring:
//   Button COM  →  ESP32 GND
//   Button NO   →  ESP32 GPIO 4
//
// Usage:
//   1. Set WLED_IP to your WLED ESP32's home network IP
//      (check WLED app → WiFi Settings, or your router's device list)
//   2. Confirm WLED_PRESET matches a saved preset in your WLED instance
//   3. Flash to your bench ESP32
//   4. Open Serial Monitor at 115200
//   5. Press button → serial shows HTTP GET sent → WLED runs preset

#include <WiFi.h>
#include <HTTPClient.h>

// ── Network (home WiFi) ───────────────────────────────────────
const char* WIFI_SSID = "Parallel";
const char* WIFI_PASS = "parallelo";

// Set this to your WLED ESP32's IP on the home network.
// Find it in: WLED web UI → Config → WiFi, or check your router.
const char* WLED_IP     = "192.168.X.X";   // <-- UPDATE THIS
const int   WLED_PRESET = 1;               // <-- match a saved WLED preset number

// ── Pin ───────────────────────────────────────────────────────
const int BUTTON_PIN = 4;   // INPUT_PULLUP — LOW when pressed

// ── Timing ────────────────────────────────────────────────────
const unsigned long AUDIO_DURATION_MS = 30000;  // retrigger lock window (ms)
const unsigned long DEBOUNCE_MS       = 50;

// ── State ─────────────────────────────────────────────────────
bool          locked       = false;
unsigned long lockStartMs  = 0;
unsigned long pressOnsetMs = 0;
bool          lastRaw      = false;

// ── WiFi ──────────────────────────────────────────────────────
void connectWiFi() {
  Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < 15000) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[WiFi] Connected. IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("[WiFi] FAILED — check SSID/password");
  }
}

// ── WLED trigger ──────────────────────────────────────────────
void triggerWLED() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WLED] skip — WiFi down");
    return;
  }
  HTTPClient http;
  String url = String("http://") + WLED_IP + "/win&PL=" + WLED_PRESET;
  http.begin(url);
  int code = http.GET();
  http.end();
  Serial.printf("[WLED] GET %s → %d\n", url.c_str(), code);
}

void startSequence() {
  locked     = true;
  lockStartMs = millis();
  Serial.println("[SEQ] Sequence start — lock ON");
  triggerWLED();
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  connectWiFi();
  lastRaw = (digitalRead(BUTTON_PIN) == LOW);
  Serial.println("[SEQ] Ready. Press button to trigger.");
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  if (locked && (millis() - lockStartMs >= AUDIO_DURATION_MS)) {
    locked = false;
    Serial.println("[SEQ] Lock cleared — idle");
  }

  if (locked) return;

  bool raw = (digitalRead(BUTTON_PIN) == LOW);

  if (raw && !lastRaw) {
    pressOnsetMs = millis();
  }

  if (raw && pressOnsetMs > 0 && (millis() - pressOnsetMs >= DEBOUNCE_MS)) {
    pressOnsetMs = 0;
    startSequence();
  }

  if (!raw) pressOnsetMs = 0;
  lastRaw = raw;
}
