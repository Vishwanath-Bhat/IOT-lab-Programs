 
#define DHTPIN 4          // Pin connected to the DHT sensor (G4 on ESP32)
#define DHTTYPE DHT11     // DHT 11 sensor type

#include <WiFi.h>
#include <DHT.h>
#include <FirebaseESP32.h>

// WiFi credentials
const char* ssid = "Mellow";         // Replace with your WiFi SSID
const char* password = "vishwa1234"; // Replace with your WiFi password

// Firebase configuration
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// Firebase credentials
 // Your Firebase Database secret
#define FIREBASE_HOST "weather-monitoring-95ee5-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Your Firebase Project URL
#define FIREBASE_AUTH "yaSDAKRc3VIrXprCecHSjNoyZUhVOAQqHEwMkYsq"

DHT dht(DHTPIN, DHTTYPE);

// Define LED pin
#define LED_PIN 2  // Pin connected to the LED

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  // Initialize the DHT sensor
  dht.begin();

  // Set the LED pin as output and turn it off initially
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up Firebase configuration
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);  // Automatically reconnect to WiFi

  Serial.println(F("Firebase initialized!"));
}

void loop() {
  // Check if the WiFi is still connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(5000);
      Serial.print(".");
    }
    Serial.println("\nConnected to WiFi.");
  }

  // Reading temperature and humidity data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from the DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C  Heat index: "));
  Serial.print(heatIndex);
  Serial.println(F("°C"));

  // Push data to Firebase
  if (Firebase.setFloat(firebaseData, "/sensor/humidity", humidity)) {
    Serial.println("Humidity uploaded successfully.");
  } else {
    Serial.println("Failed to upload humidity to Firebase.");
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, "/sensor/temperature", temperature)) {
    Serial.println("Temperature uploaded successfully.");
  } else {
    Serial.println("Failed to upload temperature to Firebase.");
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, "/sensor/heatIndex", heatIndex)) {
    Serial.println("Heat Index uploaded successfully.");
  } else {
    Serial.println("Failed to upload heat index to Firebase.");
    Serial.println(firebaseData.errorReason());
  }

  // TASK 2: Trigger an event when temperature exceeds a threshold
  if (temperature > 25.00) {
    if (Firebase.setString(firebaseData, "/alerts/tempAlert", "Temperature is high")) {
      Serial.println("Temperature alert triggered.");
    } else {
      Serial.println("Failed to trigger temperature alert.");
      Serial.println(firebaseData.errorReason());
    }
  }

  delay(500);  // Delay before next sensor read
}
