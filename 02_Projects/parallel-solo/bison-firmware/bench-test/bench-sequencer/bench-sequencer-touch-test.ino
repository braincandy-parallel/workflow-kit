// Bison Installation — ESP32 Capacitive Touch Test
//
// Purpose: find the right TOUCH_THRESHOLD for your electrode geometry
// before locking a value into the production sequencer.
//
// GPIO 4 = T0 (Touch pad 0) — same pin as the production sequencer.
//
// Wiring:
//   Run a wire from GPIO 4 to whatever you're using as the electrode
//   (bare copper wire, foil strip, the actual snout/horn copper).
//   No other connections needed.
//
// Usage:
//   1. Flash to the sequencer ESP32
//   2. Open Serial Monitor at 115200 baud
//   3. Watch raw= values with nothing touching the electrode — that's your baseline
//   4. Touch the electrode — note how far the value drops
//   5. Set TOUCH_THRESHOLD to a value between baseline and touched reading
//   6. Verify TOUCHED / idle output is reliable before moving to production

#define TOUCH_PIN        4
#define TOUCH_THRESHOLD  40    // tune this: baseline ~60-80, touched ~10-30 (lower = more capacitance)

#define SAMPLE_INTERVAL_MS  100

void setup() {
  Serial.begin(115200);
  Serial.println("[TOUCH] ESP32 capacitive touch test — GPIO 4 (T0)");
  Serial.printf("[TOUCH] Threshold: %d  (lower raw value = touch detected)\n", TOUCH_THRESHOLD);
  Serial.println("[TOUCH] Watch baseline with no touch, then touch electrode.");
  Serial.println("---");
}

void loop() {
  uint16_t val = touchRead(TOUCH_PIN);
  bool touched = (val < TOUCH_THRESHOLD);

  Serial.printf("raw=%-4u  threshold=%-4d  %s\n",
    val,
    TOUCH_THRESHOLD,
    touched ? "<<< TOUCHED" : "idle"
  );

  delay(SAMPLE_INTERVAL_MS);
}
