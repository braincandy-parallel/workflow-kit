// Bison Installation — Bench Test Sequencer
// Board: ESP32-WROOM-32E
// Partition: Minimal SPIFFS (1.9MB APP with OTA) — required for OTA + WebServer
//
// DIFFERENCES FROM PRODUCTION (sequencer.ino):
//   - Joins home WiFi (STA mode) instead of acting as AP
//   - Button on GPIO 4 with INPUT (HIGH = pressed, matches TTP223 logic)
//   - WLED_IP points to home network address of your WLED ESP32
//   - Sends WLED off preset after WLED_OFF_DELAY_MS
//
// Web config: connect device to same WiFi → open http://<IP shown in serial>
// OTA: same WiFi → Arduino IDE → Tools → Port → network:bison-bench-seq
// OTA password: bison2026
//
// Wiring:
//   Button COM  →  ESP32 GND
//   Button NO   →  ESP32 GPIO 4

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <Preferences.h>

// ── Network (home WiFi) ───────────────────────────────────────
const char* WIFI_SSID = "Cabana 6 2.4G";
const char* WIFI_PASS = "Cabana62023";

// Home network IP of WLED ESP32. Check WLED app → Config → WiFi, or router device list.
const char* WLED_IP = "192.168.1.178";

// ── Timing defaults ───────────────────────────────────────────
// Adjust via web config at http://<device IP> — no reflash needed.
const unsigned long AUDIO_DURATION_MS_DEFAULT = 22000;
const unsigned long WLED_OFF_DELAY_MS_DEFAULT = 22000;
const unsigned long DEBOUNCE_MS               = 50;

// ── WLED presets ──────────────────────────────────────────────
const int WLED_ON_PRESET  = 2;   // preset triggered on touch
const int WLED_OFF_PRESET = 1;   // preset triggered at end of sequence (off/idle)

// ── Pins ──────────────────────────────────────────────────────
const int BUTTON_PIN = 4;
const int UART_TX_PIN = 17;
const int UART_RX_PIN = 16;

// ── Runtime config (loaded from NVS on boot) ──────────────────
unsigned long audioDurationMs = AUDIO_DURATION_MS_DEFAULT;
unsigned long wledOffDelayMs  = WLED_OFF_DELAY_MS_DEFAULT;

// ── State ─────────────────────────────────────────────────────
bool          locked       = false;
bool          offSent      = false;
unsigned long lockStartMs  = 0;
unsigned long pressOnsetMs = 0;
bool          lastRaw      = false;

WebServer   server(80);
Preferences prefs;

// ── Config persistence ────────────────────────────────────────
void loadConfig() {
  prefs.begin("bison", true);
  audioDurationMs = prefs.getULong("audioDur", AUDIO_DURATION_MS_DEFAULT);
  wledOffDelayMs  = prefs.getULong("wledOff",  WLED_OFF_DELAY_MS_DEFAULT);
  prefs.end();
  Serial.printf("[CFG] audioDurationMs=%lu  wledOffDelayMs=%lu\n", audioDurationMs, wledOffDelayMs);
}

void saveConfig() {
  prefs.begin("bison", false);
  prefs.putULong("audioDur", audioDurationMs);
  prefs.putULong("wledOff",  wledOffDelayMs);
  prefs.end();
}

// ── Web config ────────────────────────────────────────────────
void handleRoot() {
  String ip = WiFi.localIP().toString();
  String html =
    "<!DOCTYPE html><html><head>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<style>"
    "body{font-family:sans-serif;max-width:420px;margin:2em auto;padding:0 1em}"
    "label{font-size:.85em;color:#555}"
    "input[type=number]{width:100%;padding:8px;margin:4px 0 4px;box-sizing:border-box;font-size:1em}"
    "small{color:#888;display:block;margin-bottom:14px}"
    "input[type=submit]{width:100%;padding:12px;background:#222;color:#fff;border:none;font-size:1em;cursor:pointer;margin-top:6px}"
    "hr{margin:1.5em 0;border:none;border-top:1px solid #ddd}"
    "p.note{color:#aaa;font-size:.8em}"
    "</style></head><body>"
    "<h2>Bison Bench Sequencer</h2>"
    "<form method='POST' action='/save'>"
    "<label>Sequence lock duration (ms)</label>"
    "<input type='number' name='dur' value='" + String(audioDurationMs) + "' min='1000' step='100'>"
    "<small>Retrigger lock window. Set to match audio file duration.</small>"
    "<label>WLED off delay (ms)</label>"
    "<input type='number' name='off' value='" + String(wledOffDelayMs) + "' min='1000' step='100'>"
    "<small>Time after trigger to send off preset. Usually same as lock duration.</small>"
    "<input type='submit' value='Save &amp; Apply'>"
    "</form>"
    "<hr>"
    "<p class='note'>OTA: Arduino IDE &rarr; Port &rarr; <b>network:bison-bench-seq</b> &rarr; password: bison2026</p>"
    "<p class='note'>Device IP: " + ip + "</p>"
    "</body></html>";
  server.send(200, "text/html", html);
}

void handleSave() {
  if (server.hasArg("dur")) {
    unsigned long val = server.arg("dur").toInt();
    if (val > 0) audioDurationMs = val;
  }
  if (server.hasArg("off")) {
    unsigned long val = server.arg("off").toInt();
    if (val > 0) wledOffDelayMs = val;
  }
  saveConfig();
  String html =
    "<!DOCTYPE html><html><head>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<style>body{font-family:sans-serif;max-width:420px;margin:2em auto;padding:0 1em}</style>"
    "</head><body>"
    "<h2>Saved</h2>"
    "<p>Lock duration: <b>" + String(audioDurationMs) + " ms</b></p>"
    "<p>WLED off delay: <b>" + String(wledOffDelayMs) + " ms</b></p>"
    "<p><a href='/'>Back</a></p>"
    "</body></html>";
  server.send(200, "text/html", html);
  Serial.printf("[CFG] Saved audioDurationMs=%lu  wledOffDelayMs=%lu\n", audioDurationMs, wledOffDelayMs);
}

// ── OTA ───────────────────────────────────────────────────────
void setupOTA() {
  ArduinoOTA.setHostname("bison-bench-seq");
  ArduinoOTA.setPassword("bison2026");
  ArduinoOTA.onStart([]()  { Serial.println("[OTA] Start"); });
  ArduinoOTA.onEnd([]()    { Serial.println("[OTA] Done — rebooting"); });
  ArduinoOTA.onError([](ota_error_t err) { Serial.printf("[OTA] Error %u\n", err); });
  ArduinoOTA.begin();
  Serial.println("[OTA] Ready — hostname: bison-bench-seq");
}

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
    Serial.printf("[HTTP] Config at http://%s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("[WiFi] FAILED — check SSID/password");
  }
}

// ── WLED trigger ──────────────────────────────────────────────
void triggerWLED(int preset) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WLED] skip — WiFi down");
    return;
  }
  HTTPClient http;
  String url = String("http://") + WLED_IP + "/win&PL=" + preset;
  http.begin(url);
  int code = http.GET();
  http.end();
  Serial.printf("[WLED] GET %s → %d\n", url.c_str(), code);
}

void startSequence() {
  locked      = true;
  offSent     = false;
  lockStartMs = millis();
  Serial.println("[SEQ] Sequence start — lock ON");
  Serial2.write('T');
  Serial.println("[SEQ] UART 'T' sent to audio ESP32");
  triggerWLED(WLED_ON_PRESET);
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  Serial.println("[SEQ] UART2 ready on GPIO 17");
  pinMode(BUTTON_PIN, INPUT);

  connectWiFi();
  loadConfig();
  setupOTA();

  server.on("/",     HTTP_GET,  handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();

  lastRaw = (digitalRead(BUTTON_PIN) == HIGH);
  Serial.println("[SEQ] Ready. Touch sensor or button to trigger.");
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  if (locked && (millis() - lockStartMs >= audioDurationMs)) {
    locked = false;
    Serial.println("[SEQ] Lock cleared — idle");
  }

  if (locked && !offSent && (millis() - lockStartMs >= wledOffDelayMs)) {
    offSent = true;
    Serial.println("[SEQ] WLED off");
    triggerWLED(WLED_OFF_PRESET);
  }

  if (locked) return;

  bool raw = (digitalRead(BUTTON_PIN) == HIGH);

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
