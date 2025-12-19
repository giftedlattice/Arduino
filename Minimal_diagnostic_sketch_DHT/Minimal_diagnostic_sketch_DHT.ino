#include <DHT.h>

#define DHT_PIN 3
#define DHTTYPE DHT11  // change to DHT22 if needed

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT test starting...");
}

void loop() {
  float h = dht.readHumidity();
  float tC = dht.readTemperature();

  if (isnan(h) || isnan(tC)) {
    Serial.println("DHT read failed");
  } else {
    Serial.print("RH=");
    Serial.print(h, 1);
    Serial.print("%  T=");
    Serial.print(tC, 1);
    Serial.println("C");
  }

  delay(2000);
}
