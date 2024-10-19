#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3GbPCit5b"
#define BLYNK_TEMPLATE_NAME "Lighting"
#define BLYNK_AUTH_TOKEN "EwpMlTKOHEGzwB1JLymUi3PS6RO7MyKy"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// LDR sensor pin
#define LDR_PIN 34  // Analog pin for LDR (e.g., GPIO 34 on ESP32)

// Define LED pin
#define LED_PIN 2  // Digital pin for LED

int ldrValue = 0;
int threshold = 200;  // Adjust based on your environment

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Mellow";  // Type your WiFi name
char pass[] = "vishwa1234";  // Type your WiFi password

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  Blynk.begin(auth, ssid, pass);

  pinMode(LDR_PIN, INPUT);  // Initialize the LDR sensor pin as an input
  pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially
  
  Serial.println(F("Smart lighting system initialized."));
  WiFi.mode(WIFI_STA);   
}

void loop() {
  Blynk.run();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Read the LDR sensor value
  ldrValue = analogRead(LDR_PIN);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Sending LDR value to Blynk app (Virtual Pin V1)
  Blynk.virtualWrite(V1, ldrValue);

  // Check if it's dark or bright and control the LED
  if (ldrValue > threshold) {  // It's dark, turn on the LED
    Serial.println(F("Dark detected, turning on LED"));
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V0, 1);  // Send status to Blynk virtual pin V0
  } else {  // It's bright, turn off the LED
    Serial.println(F("Bright detected, turning off LED"));
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V0, 0);  // Send status to Blynk virtual pin V0
  }

  delay(1000);  // Wait for 1 second before the next loop
}

BLYNK_WRITE(V2) {  // Button Widget is writing to pin V2
  int buttonState = param.asInt();

  Serial.print(F("Button state: "));
  Serial.println(buttonState);

  if (buttonState == 1) {  // If button is pressed
    Serial.println(F("Button pressed"));
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Serial.println(F("Button released"));
  }
}
