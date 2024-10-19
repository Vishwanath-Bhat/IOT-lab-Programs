#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3GgAuiVpk"
#define BLYNK_TEMPLATE_NAME "Irrigation"
#define BLYNK_AUTH_TOKEN "Cgfx6igojf73ptSHisJ3Knpw50nR9240"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Soil moisture sensor pin
#define SOIL_MOISTURE_PIN 34  // Analog pin for soil moisture sensor (e.g., GPIO 34 on ESP32)

// Define LED pin for irrigation system
#define LED_PIN 2  // LED pin to indicate irrigation status

int soilMoistureValue = 0;
int threshold = 600;  // Adjust based on your soil moisture sensor's reading in dry soil

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Mellow";  // Type your WiFi name
char pass[] = "vishwa1234";  // Type your WiFi password

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  Blynk.begin(auth, ssid, pass);

  pinMode(SOIL_MOISTURE_PIN, INPUT);  // Initialize the soil moisture sensor pin as an input
  pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially
  
  Serial.println(F("Smart irrigation system initialized."));
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

  // Read the soil moisture sensor value
  soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);

  // Sending soil moisture value to Blynk app (Virtual Pin V1)
  Blynk.virtualWrite(V1, soilMoistureValue);

  // Check if the soil is dry or wet and control the LED (representing the irrigation system)
  if (soilMoistureValue > threshold) {  // Soil is dry, turn on the irrigation (LED)
    Serial.println(F("Soil is dry, turning on irrigation"));
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V0, 1);  // Send status to Blynk virtual pin V0
  } else {  // Soil is wet, turn off the irrigation (LED)
    Serial.println(F("Soil is wet, turning off irrigation"));
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V0,0);  // Send status to Blynk virtual pin V0
  }

  delay(1000);  // Wait for 1 second before the next loop
}

BLYNK_WRITE(V2) {  // Slider Widget on Virtual Pin V2 to adjust moisture threshold
  threshold = param.asInt();
  Serial.print("Moisture Threshold set to: ");
  Serial.println(threshold);
}