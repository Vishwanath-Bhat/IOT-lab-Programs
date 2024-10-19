#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3CPxHUy2w"
#define BLYNK_TEMPLATE_NAME "iot skill"
#define BLYNK_AUTH_TOKEN "7ge870OFntFW7vkBTHt2Jy_2snzA-kWC"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// IR sensor pin
#define IR_SENSOR_PIN 4  // Pin connected to the IR sensor (e.g., G4 on ESP32)

// Define LED pin
#define LED_PIN 2  // Pin connected to the LED

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = " ";  // Type your WiFi name
char pass[] = "  ";  // Type your WiFi password

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  Blynk.begin(auth, ssid, pass);

  pinMode(IR_SENSOR_PIN, INPUT);  // Initialize the IR sensor pin as an input
  pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially
  
  Serial.println(F("Intrusion detection system initialized."));
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

  // Read the IR sensor value
  int irSensorValue = digitalRead(IR_SENSOR_PIN);

  // Check if an intrusion is detected
  if (irSensorValue == LOW) {  // Assuming LOW indicates an intrusion
    Serial.println(F("Intrusion detected!"));
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    Blynk.virtualWrite(V0, "Intrusion detected!");  // Send alert to Blynk virtual pin V0
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Blynk.virtualWrite(V0, "All clear");  // Send status to Blynk virtual pin V0
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
