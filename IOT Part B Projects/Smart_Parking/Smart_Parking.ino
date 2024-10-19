#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "Mellow"
#define WIFI_PASSWORD "vishwa1234"
#define FIREBASE_HOST "smartparking-6ed51-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "YH3ap3T4Fre5YCTXj3bbvDKp4MwHdjLqXXBjS7OA"
String FIREBASE_BUCKET = "parking"; 

// Define the GPIO pins for the IR sensors
#define IR_SENSOR1_PIN 2
#define IR_SENSOR2_PIN 5

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Set GPIO pins as input for the IR sensors
  pinMode(IR_SENSOR1_PIN, INPUT);
  pinMode(IR_SENSOR2_PIN, INPUT);
}

void loop() {
  // Read the IR sensor values
  bool spot1Occupied = digitalRead(IR_SENSOR1_PIN);
  bool spot2Occupied = digitalRead(IR_SENSOR2_PIN);

  // Print sensor values for debugging
  Serial.print("Spot 1 sensor value: ");
  Serial.println(spot1Occupied);
  Serial.print("Spot 2 sensor value: ");
  Serial.println(spot2Occupied);

  // Update the data in Firebase for Spot 1
  if (Firebase.setInt(firebaseData, String(FIREBASE_BUCKET) + "/spot1", spot1Occupied)) {
    Serial.println("Spot 1 status updated successfully");
    Serial.println("Firebase path: " + String(FIREBASE_BUCKET) + "/spot1");
    Serial.print("Value sent to Firebase: ");
    Serial.println(spot1Occupied);
  } else {
    Serial.println("Failed to update Spot 1: " + firebaseData.errorReason());
  }

  // Update the data in Firebase for Spot 2
  if (Firebase.setInt(firebaseData, String(FIREBASE_BUCKET) + "/spot2", spot2Occupied)) {
    Serial.println("Spot 2 status updated successfully");
    Serial.println("Firebase path: " + String(FIREBASE_BUCKET) + "/spot2");
    Serial.print("Value sent to Firebase: ");
    Serial.println(spot2Occupied);
  } else {
    Serial.println("Failed to update Spot 2: " + firebaseData.errorReason());
  }

  delay(1000); // Delay between checks
}
