#include <DHT.h>

#define PIR_PIN   5
#define DHT_PIN   3
#define DHTTYPE   DHT11

DHT dht(DHT_PIN, DHTTYPE);

int lastPirState = LOW;

// DHT timing
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
  Serial.println("READY_MOTION_EPISODES");
}

void updateDhtCacheIfNeeded() {
  unsigned long now = millis();
  if (now - lastDhtReadMs >= DHT_MIN_INTERVAL_MS) {
    lastDhtReadMs = now;

    float h  = dht.readHumidity();
    float tC = dht.readTemperature();
    float tF = dht.readTemperature(true);

    if (!isnan(h) && !isnan(tC) && !isnan(tF)) {
      lastHumidity = h;
      lastTempC    = tC;
      lastTempF    = tF;
    }
  }
}

void printMotionCsv(unsigned long ms) {
  Serial.print("MOTION,");
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

  // keep DHT cache reasonably fresh
  updateDhtCacheIfNeeded();

  // Mirror PIR directly to onboard LED
  digitalWrite(LED_BUILTIN, pirState == HIGH ? HIGH : LOW);

  // Rising edge: motion trigger event
  if (pirState == HIGH && lastPirState == LOW) {
    printMotionCsv(now);
  }

  lastPirState = pirState;
}
