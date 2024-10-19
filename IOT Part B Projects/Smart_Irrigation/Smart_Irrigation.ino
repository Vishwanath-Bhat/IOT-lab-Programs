#define DHTPIN 4          // Pin connected to the DHT sensor (G4 on ESP32)
#define DHTTYPE DHT11     // DHT 11 sensor type
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>   // Include the ESP32Servo library

#define FIREBASE_HOST "smart-irrigation-67c9d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "z62GKiEdS8td4q1RokrdSx68QD797uNaoUe2cFCH"
#define WIFI_SSID "Rekha 5g"
#define WIFI_PASSWORD "9448051890rekha"

DHT dht(DHTPIN, DHTTYPE);
Servo valveServo;         
FirebaseData firebaseData;

String FIREBASE_BUCKET = "irrigation"; 

int humidity = 0;
int temp = 0;
int servoPin = 5;

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  dht.begin();

  // Attach the servo to the specified pin using ESP32Servo
  valveServo.attach(servoPin);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void readHumidity() {
  humidity = dht.readHumidity();
}

void readTemp() {
  temp = dht.readTemperature();
}

void controlValve(int humidity, int temp) {
  bool isValveOpen = false; // Variable to store the valve status

  if (humidity < 40 || temp > 30) {  // Example condition: if humidity is below 40% or temp above 30°C
    valveServo.write(90);  // Turn the valve to 90 degrees (open position)
    isValveOpen = true;
    Serial.println("Valve opened");
  } else {
    valveServo.write(0);   // Turn the valve to 0 degrees (closed position)
    isValveOpen = false;
    Serial.println("Valve closed");
  }

  // Send valve status to Firebase
  Firebase.setBool(firebaseData, String(FIREBASE_BUCKET) + "/water", isValveOpen);
}

void loop() {
  readHumidity();
  readTemp();
  Serial.println("Current Humidity: " + String(humidity) + " and Temperature: " + String(temp));
  
  // Send temperature and humidity data to Firebase
  Firebase.setInt(firebaseData, String(FIREBASE_BUCKET) + "/humidity", humidity);
  Firebase.setInt(firebaseData, String(FIREBASE_BUCKET) + "/temperature", temp);
  
  // Control the valve based on the readings
  controlValve(humidity, temp);
  
  delay(1000); // Delay between checks
}
