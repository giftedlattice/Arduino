#define PIR_PIN 5

int lastState = LOW;
unsigned long lastPrint = 0;
const unsigned long PRINT_INTERVAL_MS = 200;  // how often we print current state

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("PIR debug started.");
  Serial.println("Columns: millis, raw_state(0/1), state_str");
}

void loop() {
  int state = digitalRead(PIR_PIN);
  unsigned long now = millis();

  // Mirror PIR directly to onboard LED
  digitalWrite(LED_BUILTIN, state == HIGH ? HIGH : LOW);

  // Print whenever the state changes
  if (state != lastState) {
    Serial.print(now);
    Serial.print(", ");
    Serial.print(state);
    Serial.print(", ");
    Serial.println(state == HIGH ? "HIGH (MOTION)" : "LOW (NO MOTION)");

    lastState = state;
  }

  // Optionally: also print the current state periodically, even if it didn't change
  if (now - lastPrint >= PRINT_INTERVAL_MS) {
    lastPrint = now;
    Serial.print(now);
    Serial.print(", ");
    Serial.print(state);
    Serial.print(", ");
    Serial.println(state == HIGH ? "HIGH" : "LOW");
  }

  // small delay so we don't spam too hard
  delay(10);
}
