#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3CPxHUy2w"
#define BLYNK_TEMPLATE_NAME "iot skill"
#define BLYNK_AUTH_TOKEN "7ge870OFntFW7vkBTHt2Jy_2snzA-kWC"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Smoke and flame sensor pins
#define SMOKE_SENSOR_PIN 34  // Analog pin connected to the smoke sensor (e.g., GPIO 34 on ESP32)
#define FLAME_SENSOR_PIN 35  // Analog pin connected to the flame sensor (e.g., GPIO 35 on ESP32)

// Define LED pin for alarm indication
#define LED_PIN 2  // Digital pin for LED

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = " ";  // Type your WiFi name
char pass[] = "  ";  // Type your WiFi password

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  Blynk.begin(auth, ssid, pass);

  pinMode(SMOKE_SENSOR_PIN, INPUT);  // Initialize the smoke sensor pin as an input
  pinMode(FLAME_SENSOR_PIN, INPUT);  // Initialize the flame sensor pin as an input
  pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially
  
  Serial.println(F("Forest fire detection system initialized."));
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

  // Read the smoke and flame sensor values
  int smokeValue = analogRead(SMOKE_SENSOR_PIN);
  int flameValue = analogRead(FLAME_SENSOR_PIN);

  Serial.print("Smoke Sensor Value: ");
  Serial.println(smokeValue);
  Serial.print("Flame Sensor Value: ");
  Serial.println(flameValue);

  // Sending sensor values to Blynk app
  Blynk.virtualWrite(V0, smokeValue);  // Send smoke sensor value to Blynk virtual pin V0
  Blynk.virtualWrite(V1, flameValue);  // Send flame sensor value to Blynk virtual pin V1

  // Check for smoke or flame detection
  if (smokeValue > 300 || flameValue > 300) {  // Threshold values for detection (adjust as needed)
    Serial.println(F("Fire detected!"));
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    Blynk.virtualWrite(V2, "Fire Detected!");  // Send alert to Blynk virtual pin V2
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Blynk.virtualWrite(V2, "No Fire");  // Send status to Blynk virtual pin V2
  }

  delay(1000);  // Wait for 1 second before the next loop
}

BLYNK_WRITE(V3) {  // Button Widget on Virtual Pin V3 to manually control the LED (alarm)
  int buttonState = param.asInt();

  Serial.print(F("Button state: "));
  Serial.println(buttonState);

  if (buttonState == 1) {  // If button is pressed
    Serial.println(F("Manually turning on LED"));
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
  } else {
    Serial.println(F("Manually turning off LED"));
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
  }
}
