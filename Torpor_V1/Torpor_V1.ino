#include <DHT.h>

#define PIR_PIN   5
#define DHT_PIN   3
#define DHTTYPE   DHT11

DHT dht(DHT_PIN, DHTTYPE);

int lastPirState = LOW;
unsigned long lastDhtReadMs = 0;

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);          // PIR output -> Arduino input
  pinMode(LED_BUILTIN, OUTPUT);     // optional indicator

  dht.begin();
  Serial.println("Ready. Waiting for motion...");
}

void loop() {
  int pirState = digitalRead(PIR_PIN);

  // Detect motion start (LOW -> HIGH)
  if (pirState == HIGH && lastPirState == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);

    // DHT11 should not be read too fast; 2s is a safe interval
    unsigned long now = millis();
    if (now - lastDhtReadMs >= 2000) {
      lastDhtReadMs = now;

      float humidity = dht.readHumidity();
      float tempC = dht.readTemperature();        // Celsius
      float tempF = dht.readTemperature(true);    // Fahrenheit

      if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
        Serial.println("DHT read failed (try again on next motion).");
      } else {
        Serial.print("Motion!  Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temp: ");
        Serial.print(tempC);
        Serial.print("C / ");
        Serial.print(tempF);
        Serial.println("F");
      }
    } else {
      Serial.println("Motion! (Skipping DHT read: too soon since last read)");
    }
  }

  // Optional: turn LED off when motion signal goes low
  if (pirState == LOW && lastPirState == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  }

  lastPirState = pirState;
}
