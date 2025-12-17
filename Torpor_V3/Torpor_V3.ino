#include <DHT.h>

#define PIR_PIN   5
#define DHT_PIN   3
#define DHTTYPE   DHT11

DHT dht(DHT_PIN, DHTTYPE);

int lastPirState = LOW;
unsigned long lastDhtReadMs = 0;
const unsigned long DHT_MIN_INTERVAL_MS = 2000;  // 2 s

// cache last good DHT readings
float lastHumidity = NAN;
float lastTempC    = NAN;
float lastTempF    = NAN;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  dht.begin();
  Serial.println("READY_START_END");
}

void updateDhtCacheIfNeeded() {
  unsigned long now = millis();
  if (now - lastDhtReadMs >= DHT_MIN_INTERVAL_MS) {
    lastDhtReadMs = now;

    float h = dht.readHumidity();
    float tC = dht.readTemperature();
    float tF = dht.readTemperature(true);

    if (!isnan(h) && !isnan(tC) && !isnan(tF)) {
      lastHumidity = h;
      lastTempC    = tC;
      lastTempF    = tF;
    }
  }
}

void printDhtCsv(unsigned long ms) {
  Serial.print(ms);
  Serial.print(",");

  if (isnan(lastHumidity) || isnan(lastTempC) || isnan(lastTempF)) {
    Serial.print("NA,NA,NA");
  } else {
    Serial.print(lastHumidity, 1);
    Serial.print(",");
    Serial.print(lastTempC, 1);
    Serial.print(",");
    Serial.print(lastTempF, 1);
  }
  Serial.println();
}

void loop() {
  int pirState = digitalRead(PIR_PIN);
  unsigned long now = millis();

  // Always keep DHT cache reasonably fresh
  updateDhtCacheIfNeeded();

  // Rising edge: motion started
  if (pirState == HIGH && lastPirState == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.print("MOTION_START,");
    printDhtCsv(now);
  }

  // Falling edge: motion ended
  if (pirState == LOW && lastPirState == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);

    Serial.print("MOTION_END,");
    printDhtCsv(now);
  }

  lastPirState = pirState;
}
