#define FIREBASE_HOST "intrusion-detection-20d54-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Replace with your Firebase Realtime Database URL
#define FIREBASE_AUTH "0yNXWrPbjOoKC3OtAhIzcQlhb8nsWGR4Y3Jar7yC"  // Replace with your Firebase Database secret or token

#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>

// Smoke sensor pin
#define SMOKE_SENSOR_PIN 34  // Analog pin connected to the smoke sensor (e.g., GPIO 34 on ESP32)

// Define LED pin for alarm indication
#define LED_PIN 2  // Digital pin for LED

// WiFi credentials
char ssid[] = "Mellow";  // Type your WiFi name
char pass[] = "vishwa1234";  // Type your WiFi password

// Firebase Data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);

  pinMode(SMOKE_SENSOR_PIN, INPUT);  // Initialize the smoke sensor pin as an input
  pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially
  
  Serial.println(F("Forest fire detection system initialized."));
}

void loop() {
  // Ensure the WiFi connection is active
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);  // Reconnect if disconnected
  }

  // Read the smoke sensor value
  int smokeValue = analogRead(SMOKE_SENSOR_PIN);
  Serial.print("Smoke Sensor Value: ");
  Serial.println(smokeValue);

  // Initialize the fire status message
  String fireStatus;

  // Check for smoke detection
  if (smokeValue > 2300) {  // Threshold value for detection (adjust as needed)
    Serial.println(F("Smoke detected!"));
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    fireStatus = "Fire Detected!";
  } else {
    Serial.println(F("No smoke detected."));
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    fireStatus = "No Fire";
  }

  // Push both the smoke value and fire status to Firebase
  if (Firebase.setInt(firebaseData, "/sensorValues/smokeValue", smokeValue)) {
    Serial.println(F("Smoke value updated in Firebase"));
  } else {
    Serial.print(F("Failed to update smoke value: "));
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.setString(firebaseData, "/sensorValues/fireStatus", fireStatus)) {
    Serial.println(F("Fire status updated in Firebase"));
  } else {
    Serial.print(F("Failed to update fire status: "));
    Serial.println(firebaseData.errorReason());
  }

  delay(1000);  // Wait for 1 second before the next loop
}
