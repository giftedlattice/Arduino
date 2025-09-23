//maximum precision and responsiveness, the next step is to use hardware timers on the Arduino.
//usually called:
//Timer/Counter with Output Compare
//Or simply hardware-timed pulse generation


// ---------- Pins ----------
const uint8_t BTN1_PIN = 2;    // Button 1: triggers 3 device pulses
const uint8_t BTN2_PIN = 3;    // Button 2: LED flash + 1 device pulse
const uint8_t TTL1_PIN = 8;    // Device 1
const uint8_t TTL2_PIN = 9;    // Device 2
const uint8_t TTL3_PIN = 10;   // Device 3
const uint8_t TTL4_PIN = 11;   // Device for Button 2
const uint8_t LED_PIN  = 13;   // LED

// ---------- Timing ----------
const unsigned int TTL_PULSE_US = 5000;  // TTL pulse width (µs)
const unsigned int LED_MS       = 100;   // LED on time (ms)

// ---------- Helpers ----------
inline void sendPulse(uint8_t pin, unsigned int width_us) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(width_us);  // only place delay is actually needed
  digitalWrite(pin, LOW);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(TTL1_PIN, OUTPUT);
  pinMode(TTL2_PIN, OUTPUT);
  pinMode(TTL3_PIN, OUTPUT);
  pinMode(TTL4_PIN, OUTPUT);
  pinMode(LED_PIN,  OUTPUT);

  digitalWrite(TTL1_PIN, LOW);
  digitalWrite(TTL2_PIN, LOW);
  digitalWrite(TTL3_PIN, LOW);
  digitalWrite(TTL4_PIN, LOW);
  digitalWrite(LED_PIN,  LOW);
}

void loop() {
  // --- Button 1: three device pulses, nearly simultaneous ---
  if (digitalRead(BTN1_PIN) == LOW) { // pressed
    // fire each device
    sendPulse(TTL1_PIN, TTL_PULSE_US);
    sendPulse(TTL2_PIN, TTL_PULSE_US);
    sendPulse(TTL3_PIN, TTL_PULSE_US);

    // Wait for release (prevents retrigger on hold)
    while (digitalRead(BTN1_PIN) == LOW) { }
    delay(20); // small debounce
  }

  // --- Button 2: LED flash (timed) + one device pulse ---
  if (digitalRead(BTN2_PIN) == LOW) {
    // start LED
    digitalWrite(LED_PIN, HIGH);
    unsigned long tStart = millis();

    // fire TTL
    sendPulse(TTL4_PIN, TTL_PULSE_US);

    // let LED run in background
    while (millis() - tStart < LED_MS) {
      // LED stays on; loop is blocked here briefly
    }
    digitalWrite(LED_PIN, LOW);

    // Wait for release (prevents retrigger on hold)
    while (digitalRead(BTN2_PIN) == LOW) { }
    delay(20);
  }
}
