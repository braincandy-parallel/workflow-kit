// Bison Installation — Audio ESP32
// Board: ESP32-WROOM-32E
//
// Responsibilities:
//   - Wait for UART trigger byte from Sequencer
//   - Play bison.wav from SD card via PCM5102A (I2S)
//   - No WiFi — eliminates I2S/WiFi interrupt conflicts
//
// Wiring:
//   PCM5102A  →  ESP32
//   DIN           GPIO 22  (I2S_DOUT)
//   BCK           GPIO 26  (I2S_BCLK)
//   LCK           GPIO 25  (I2S_LRC)
//   VIN           3.3V
//   GND           GND
//   SCK           GND      (no MCLK needed)
//   FMT           GND      (I2S format)
//   XSMT          3.3V     (unmute)
//   FLT           GND      (normal latency filter)
//   DEMP          GND      (no de-emphasis)
//
//   SD Module   →  ESP32
//   SCK           GPIO 18
//   MISO          GPIO 19
//   MOSI          GPIO 23
//   CS            GPIO 5
//   VCC           3.3V
//   GND           GND
//
//   Sequencer   →  ESP32
//   GPIO 17 TX    GPIO 16 RX  (UART2)

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Audio.h"   // ESP32-audioI2S by schreibfaul1

// ── I2S Pins (PCM5102A) ───────────────────────────────────────
#define I2S_DOUT  22
#define I2S_BCLK  26
#define I2S_LRC   25

// ── SD SPI Pins ───────────────────────────────────────────────
#define SD_CS     5
#define SPI_SCK   18
#define SPI_MISO  19
#define SPI_MOSI  23

// ── UART from Sequencer ───────────────────────────────────────
#define UART_RX   16
#define UART_TX   17   // unused; required for Serial2.begin

// ── Audio file ────────────────────────────────────────────────
// Filename must match exactly — FAT32, short name, no spaces.
// Update when JB delivers final file.
const char* AUDIO_FILE = "/bison.wav";

// ── Volume ────────────────────────────────────────────────────
// 0–21. 21 = maximum output level. Set to taste during dry-run.
const int VOLUME = 21;

Audio audio;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  Serial.println("[AUDIO] UART ready on GPIO 16");

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("[AUDIO] SD mount FAILED — check wiring and card format (FAT32)");
    return;
  }
  Serial.println("[AUDIO] SD ready");

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(VOLUME);
  Serial.println("[AUDIO] Ready");
}

void loop() {
  if (Serial2.available()) {
    char c = Serial2.read();
    if (c == 'T') {
      Serial.println("[AUDIO] Trigger received — playing");
      audio.connecttoFS(SD, AUDIO_FILE);
    }
  }
  audio.loop();
}

// ── Optional callbacks (useful for debugging) ─────────────────
void audio_info(const char* info) {
  Serial.printf("[AUDIO INFO] %s\n", info);
}
void audio_eof_mp3(const char* info) {
  Serial.printf("[AUDIO EOF] %s\n", info);
}
