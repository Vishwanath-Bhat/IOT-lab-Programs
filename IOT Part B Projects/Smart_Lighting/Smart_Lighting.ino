#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3VYYuFkHO"
#define BLYNK_TEMPLATE_NAME "Smart Lighting"
#define BLYNK_AUTH_TOKEN "htYACP4P4oejQHlv5HY56DYY8CSQft0u"
#define IR_SENSOR1_PIN 4

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "realme GT Master Edition";     // Type your WiFi name
char pass[] = "g7dmxhpk";   // Type your WiFi password

// Define LED pin
#define LED_PIN 2         // Pin connected to the LED

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));
  Blynk.begin(auth, ssid, pass);

  // Wait a few seconds between measurements.
  delay(2000);

  pinMode(LED_PIN, OUTPUT); // Initialize the LED pin as an output
  pinMode(IR_SENSOR1_PIN, INPUT);
  digitalWrite(LED_PIN, HIGH); // Ensure the LED is off initially
  Serial.println(F("DHTxx test!"));
}

void loop() {
  Blynk.run();
  bool spot1Occupied = digitalRead(IR_SENSOR1_PIN);
  if(spot1Occupied)
  digitalWrite(LED_PIN, HIGH);
  else
  digitalWrite(LED_PIN, LOW);

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
