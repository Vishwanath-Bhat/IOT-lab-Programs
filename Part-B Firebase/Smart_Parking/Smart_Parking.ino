
#include <WiFi.h>
#include <FirebaseESP32.h>

// Firebase project credentials
#define FIREBASE_HOST "smartparking-6ed51-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Your Firebase host
#define FIREBASE_AUTH "YH3ap3T4Fre5YCTXj3bbvDKp4MwHdjLqXXBjS7OA"  // Your Firebase secret key

// IR sensor pins
#define IR_SENSOR1_PIN 4  // Pin connected to the first IR sensor (e.g., G4 on ESP32)
#define IR_SENSOR2_PIN 5  // Pin connected to the second IR sensor (e.g., G5 on ESP32)

// Define LED pins for parking spots
#define LED1_PIN 2  // LED for the first parking spot
#define LED2_PIN 15 // LED for the second parking spot

char ssid[] = "Mellow";  // Your WiFi name
char pass[] = "vishwa1234";  // Your WiFi password

FirebaseData firebaseData;  // Firebase object to interact with

// Define Firebase config and auth objects
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize Firebase config
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(IR_SENSOR1_PIN, INPUT);  // Initialize the first IR sensor pin as an input
  pinMode(IR_SENSOR2_PIN, INPUT);  // Initialize the second IR sensor pin as an input
  pinMode(LED1_PIN, OUTPUT);  // Initialize the first LED pin as an output
  pinMode(LED2_PIN, OUTPUT);  // Initialize the second LED pin as an output
  digitalWrite(LED1_PIN, LOW);  // Ensure the first LED is off initially
  digitalWrite(LED2_PIN, LOW);  // Ensure the second LED is off initially

  Serial.println(F("Smart parking system initialized."));
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("\nReconnected to WiFi");
  }

  // Read the IR sensor values
  int irSensor1Value = digitalRead(IR_SENSOR1_PIN);
  int irSensor2Value = digitalRead(IR_SENSOR2_PIN);

  // Check if the first parking spot is occupied
  if (irSensor1Value == LOW) {  // Assuming LOW indicates a car is present
    Serial.println(F("Parking spot 1 occupied"));
    digitalWrite(LED1_PIN, HIGH);  // Turn on the LED for the first spot

    // Update Firebase
    Firebase.setString(firebaseData, "/parking_spots/spot1", "Occupied");
  } else {
    Serial.println(F("Parking spot 1 available"));
    digitalWrite(LED1_PIN, LOW);  // Turn off the LED for the first spot

    // Update Firebase
    Firebase.setString(firebaseData, "/parking_spots/spot1", "Available");
  }

  // Check if the second parking spot is occupied
  if (irSensor2Value == LOW) {  // Assuming LOW indicates a car is present
    Serial.println(F("Parking spot 2 occupied"));
    digitalWrite(LED2_PIN, HIGH);  // Turn on the LED for the second spot

    // Update Firebase
    Firebase.setString(firebaseData, "/parking_spots/spot2", "Occupied");
  } else {
    Serial.println(F("Parking spot 2 available"));
    digitalWrite(LED2_PIN, LOW);  // Turn off the LED for the second spot

    // Update Firebase
    Firebase.setString(firebaseData, "/parking_spots/spot2", "Available");
  }

  delay(1000);  // Wait for 1 second before the next loop
}
