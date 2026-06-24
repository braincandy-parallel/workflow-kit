// Bison Installation — Sequencer ESP32
// Board: ESP32-WROOM-32E
// Partition: Minimal SPIFFS (1.9MB APP with OTA) — required for OTA + WebServer
//
// Responsibilities:
//   - Read touch input (GPIO 4)
//   - Enforce retrigger lock for duration of audio file
//   - Send UART byte to Audio ESP32
//   - Send HTTP GET to WLED ESP32 to trigger lighting preset
//   - Act as WiFi AP for WLED ESP32
//   - Serve web config at http://192.168.4.1 (connect device to bison-ap)
//   - Accept OTA firmware updates over WiFi
//
// Web config: connect any phone/laptop to bison-ap → open http://192.168.4.1
// OTA: connect laptop to bison-ap → Arduino IDE → Tools → Port → network:bison-sequencer
// OTA password: bison2026

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <Preferences.h>

// ── Network ───────────────────────────────────────────────────
const char*       AP_SSID     = "bison-ap";
const char*       AP_PASS     = "bisonaudio";
const IPAddress   AP_IP(192, 168, 4, 1);
const IPAddress   AP_GATEWAY(192, 168, 4, 1);
const IPAddress   AP_SUBNET(255, 255, 255, 0);
const IPAddress   WLED_IP(192, 168, 4, 11);
const int         WLED_PRESET = 1;

// ── Pins ──────────────────────────────────────────────────────
const int PIN_TOUCH = 4;
const int UART_TX   = 17;
const int UART_RX   = 16;

// ── Timing defaults ───────────────────────────────────────────
// Set via web config at http://192.168.4.1 after JB delivers final audio.
// Value must equal exact audio file duration in milliseconds.
const unsigned long AUDIO_DURATION_MS_DEFAULT = 30000;
const unsigned long DEBOUNCE_MS               = 50;

// ── Runtime config (loaded from NVS on boot) ──────────────────
unsigned long audioDurationMs = AUDIO_DURATION_MS_DEFAULT;

// ── State ─────────────────────────────────────────────────────
bool          locked       = false;
unsigned long lockStartMs  = 0;
unsigned long touchOnsetMs = 0;

WebServer   server(80);
Preferences prefs;

// ── Config persistence ────────────────────────────────────────
void loadConfig() {
  prefs.begin("bison", true);
  audioDurationMs = prefs.getULong("audioDur", AUDIO_DURATION_MS_DEFAULT);
  prefs.end();
  Serial.printf("[CFG] audioDurationMs=%lu\n", audioDurationMs);
}

void saveConfig() {
  prefs.begin("bison", false);
  prefs.putULong("audioDur", audioDurationMs);
  prefs.end();
}

// ── Web config ────────────────────────────────────────────────
void handleRoot() {
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
    "<h2>Bison Sequencer</h2>"
    "<form method='POST' action='/save'>"
    "<label>Sequence lock duration (ms)</label>"
    "<input type='number' name='dur' value='" + String(audioDurationMs) + "' min='1000' step='100'>"
    "<small>Must match exact audio file duration. Controls retrigger lock window.</small>"
    "<input type='submit' value='Save &amp; Apply'>"
    "</form>"
    "<hr>"
    "<p class='note'>OTA: connect laptop to <b>bison-ap</b> &rarr; Arduino IDE &rarr; Port &rarr; <b>network:bison-sequencer</b> &rarr; password: bison2026</p>"
    "</body></html>";
  server.send(200, "text/html", html);
}

void handleSave() {
  if (server.hasArg("dur")) {
    unsigned long val = server.arg("dur").toInt();
    if (val > 0) audioDurationMs = val;
  }
  saveConfig();
  String html =
    "<!DOCTYPE html><html><head>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<style>body{font-family:sans-serif;max-width:420px;margin:2em auto;padding:0 1em}</style>"
    "</head><body>"
    "<h2>Saved</h2>"
    "<p>Lock duration: <b>" + String(audioDurationMs) + " ms</b></p>"
    "<p><a href='/'>Back</a></p>"
    "</body></html>";
  server.send(200, "text/html", html);
  Serial.printf("[CFG] Saved audioDurationMs=%lu\n", audioDurationMs);
}

// ── OTA ───────────────────────────────────────────────────────
void setupOTA() {
  ArduinoOTA.setHostname("bison-sequencer");
  ArduinoOTA.setPassword("bison2026");
  ArduinoOTA.onStart([]()  { Serial.println("[OTA] Start"); });
  ArduinoOTA.onEnd([]()    { Serial.println("[OTA] Done — rebooting"); });
  ArduinoOTA.onError([](ota_error_t err) { Serial.printf("[OTA] Error %u\n", err); });
  ArduinoOTA.begin();
  Serial.println("[OTA] Ready — hostname: bison-sequencer");
}

// ── Trigger functions ─────────────────────────────────────────
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
  triggerAudio();
  triggerWLED();
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

  loadConfig();
  setupOTA();

  server.on("/",     HTTP_GET,  handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  Serial.println("[HTTP] Config at http://192.168.4.1");
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  if (locked && (millis() - lockStartMs >= audioDurationMs)) {
    locked = false;
    Serial.println("[SEQ] Lock cleared — idle");
  }

  if (locked) return;

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
