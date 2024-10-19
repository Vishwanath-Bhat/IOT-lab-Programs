#define DHTPIN 4          // Pin connected to the DHT sensor (G4 on ESP32)
#define DHTTYPE DHT11     // DHT 11 sensor type
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3CPxHUy2w"
#define BLYNK_TEMPLATE_NAME "iot skill"
#define BLYNK_AUTH_TOKEN "7ge870OFntFW7vkBTHt2Jy_2snzA-kWC"

#include <WiFi.h>

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = " ";     // Type your WiFi name
char pass[] = "  ";   // Type your WiFi password

DHT dht(DHTPIN, DHTTYPE);

// Define LED pin
#define LED_PIN 2         // Pin connected to the LED

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  // Wait a few seconds between measurements.
  delay(2000);

  pinMode(LED_PIN, OUTPUT); // Initialize the LED pin as an output
  digitalWrite(LED_PIN, HIGH); // Ensure the LED is off initially
  Serial.println(F("DHTxx test!"));
    WiFi.mode(WIFI_STA);   
   
}

void loop() {
  Blynk.run();
   if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
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
  Blynk.virtualWrite(V1, h);  // Send humidity to virtual pin V5
  Blynk.virtualWrite(V0, t);  // Send temperature to virtual pin V6

  //************TODO*************************
  // TASK : 1
  // Description : Push Heat index data to iot cloud
  //******************************************

  //*******************************************
  // TASK : 2
  //Description : Trigger an event upon reaching a thershold of temperature/humidity/heat-index
  //Below is an example of simple condition were the temperature is greater than 25 degrees 
  //   raise an alert.
  // if(t > 25.00){
  //   Blynk.logEvent("tempalert","Temperature is high");
  // }
  // Note : Event definition should be created in the blynk IoT cloud before configuring it. 
  //********************************************

}
BLYNK_WRITE(V2) { // Button Widget is writing to pin V1
  int buttonState = param.asInt();
  
  Serial.print(F("Button state: "));
  Serial.println(buttonState);

  if (buttonState == 1) { // If button is pressed
    Serial.println(F("Button pressed"));
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Serial.println(F("Button released"));
  }
}