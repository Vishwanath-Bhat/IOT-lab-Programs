#define DHTPIN 4          // Pin connected to the DHT sensor (G4 on ESP32)
#define DHTTYPE DHT11     // DHT 11 sensor type
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3kCFJUUWx"
#define BLYNK_TEMPLATE_NAME "Weather Monitering"
#define BLYNK_AUTH_TOKEN "YophYsLmRm2nzP08aVaYN1yE9P25Jmll"

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "realme GT Master Edition";     // Type your WiFi name
char pass[] = "g7dmxhpk";   // Type your WiFi password

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  // Wait a few seconds between measurements.
  delay(2000);

  Serial.println(F("DHTxx test!"));
}

void loop() {
  Blynk.run();
  
  // Reading temperature and humidity data  // 
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from the DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C"));

  // Send data to Blynk
  Blynk.virtualWrite(V5, h);  // Send humidity to virtual pin V5
  Blynk.virtualWrite(V6, t);  // Send temperature to virtual pin V6

}
