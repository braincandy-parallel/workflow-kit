// Bison Installation — Bench Test Audio
// Replaces production audio.ino for home bench testing.
//
// DIFFERENCES FROM PRODUCTION (audio.ino):
//   - Plays benchtest.wav instead of bison.wav
//   - Everything else identical: same pins, same UART trigger byte ('T')
//
// Wiring (same as production):
//   PCM5102A  →  ESP32
//   DIN           GPIO 22  (I2S_DOUT)
//   BCK           GPIO 26  (I2S_BCLK)
//   LCK           GPIO 25  (I2S_LRC)
//   VIN           3.3V
//   GND           GND
//
//   SD Module   →  ESP32
//   SCK           GPIO 18
//   MISO          GPIO 19
//   MOSI          GPIO 23
//   CS            GPIO 5
//   VCC           5V
//   GND           GND
//
//   Sequencer   →  ESP32
//   GPIO 17 TX    GPIO 16 RX  (UART2)
//
// SD card setup:
//   - Format: FAT32
//   - /benchtest.wav  — main audio (48kHz stereo 16-bit)
//   - /silence.wav    — 5s silence (48kHz stereo 16-bit), loops to keep I2S clock alive

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Audio.h"       // ESP32-audioI2S by schreibfaul1
#include <driver/i2s.h>  // for i2s_zero_dma_buffer

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

const char* AUDIO_FILE   = "/benchtest.wav";
const char* SILENCE_FILE = "/silence.wav";
const int   VOLUME = 21;

Audio audio;

// Set in audio_eof_mp3; handled in loop() to avoid re-entrancy.
volatile bool pendingSilence = false;

void audio_info(const char* info) {
  Serial.printf("[LIB] %s\n", info);
}

void audio_eof_mp3(const char* info) {
  Serial.printf("[AUDIO] EOF: %s\n", info);
  // Zero all DMA buffers so the underrun window plays silence, not garbage.
  i2s_zero_dma_buffer(I2S_NUM_0);
  pendingSilence = true;
}

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

  // Start silence immediately so I2S clock is running before first trigger.
  audio.connecttoFS(SD, SILENCE_FILE);
  audio.setFileLoop(true);
  Serial.println("[AUDIO] Ready — waiting for trigger on GPIO 16");
}

void loop() {
  // Handle trigger first so it can cancel a pending silence switch.
  if (Serial2.available()) {
    char c = Serial2.read();
    if (c == 'T') {
      Serial.println("[AUDIO] Trigger received — playing benchtest.wav");
      pendingSilence = false;
      audio.connecttoFS(SD, AUDIO_FILE);
    }
  }

  // Switch back to looping silence after any track ends.
  if (pendingSilence) {
    pendingSilence = false;
    audio.connecttoFS(SD, SILENCE_FILE);
    audio.setFileLoop(true);
  }

  audio.loop();
}
