#include <DHT.h>

#define PIR_PIN   5
#define DHT_PIN   3
#define DHTTYPE   DHT11

DHT dht(DHT_PIN, DHTTYPE);

int lastPirState = LOW;
unsigned long lastDhtReadMs = 0;
const unsigned long DHT_MIN_INTERVAL_MS = 500;  // 

void setup() {
  Serial.begin(115200); // faster, more robust
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  dht.begin();
  Serial.println("READY");   // handshake string for the Pi
}

void loop() {
  int pirState = digitalRead(PIR_PIN);

  // rising edge = new motion event
  if (pirState == HIGH && lastPirState == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    
    unsigned long now = millis();
    bool doDht = (now - lastDhtReadMs >= DHT_MIN_INTERVAL_MS);
    float humidity = NAN, tempC = NAN, tempF = NAN;

    if (doDht) {
      lastDhtReadMs = now;
      humidity = dht.readHumidity();
      tempC   = dht.readTemperature();
      tempF   = dht.readTemperature(true);
    }

    // Single CSV line per motion onset.
    Serial.print("MOTION,");
    Serial.print(now);              // Arduino millis
    Serial.print(",");
    if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
      Serial.print("NA,NA,NA");
    } else {
      Serial.print(humidity, 1);    // 1 decimal
      Serial.print(",");
      Serial.print(tempC, 1);
      Serial.print(",");
      Serial.print(tempF, 1);
    }
    Serial.println();
  }

  // falling edge: clear LED (optional)
  if (pirState == LOW && lastPirState == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  }

  lastPirState = pirState;
}
