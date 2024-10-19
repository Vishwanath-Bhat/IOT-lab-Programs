#define FIREBASE_HOST "intrusion-detection-20d54-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "0yNXWrPbjOoKC3OtAhIzcQlhb8nsWGR4Y3Jar7yC"

#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>

// Ultrasonic sensor pins
#define TRIG_PIN 5  // GPIO pin connected to the Trig pin of the sensor
#define ECHO_PIN 18  // GPIO pin connected to the Echo pin of the sensor

// Define LED pin for alarm indication
#define LED_PIN 2  // Digital pin for LED

// WiFi credentials
char ssid[] = "Mellow";  // Type your WiFi name
char pass[] = "vishwa1234";  // Type your WiFi password

// Firebase Data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

long duration;
float distance;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially

  Serial.println(F("Intrusion detection system initialized."));
}

void loop() {
  // Ensure the WiFi connection is active
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
  }

  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo time
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance (speed of sound is 0.034 cm/microsecond)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // If distance is less than the threshold (e.g., 50 cm), intrusion is detected
  if (distance < 50) {
    Serial.println(F("Intrusion detected!"));
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED

    // Push the intrusion status to Firebase
    if (Firebase.setString(firebaseData, "/intrusionStatus", "Intrusion detected!")) {
      Serial.println(F("Intrusion status updated in Firebase"));
    } else {
      Serial.print(F("Failed to update intrusion status: "));
      Serial.println(firebaseData.errorReason());  // Print the error reason
    }
  } else {
    Serial.println(F("All clear"));
    digitalWrite(LED_PIN, LOW);  // Turn off the LED

    // Push the "all clear" status to Firebase
    if (Firebase.setString(firebaseData, "/intrusionStatus", "All clear")) {
      Serial.println(F("Status updated to Firebase"));
    } else {
      Serial.print(F("Failed to update status: "));
      Serial.println(firebaseData.errorReason());  // Print the error reason
    }
  }

  delay(1000);  // Wait for 1 second before the next loop
}




// #include <WiFi.h>
// #include <ThingSpeak.h>

// // Pin definitions
// const int trigPin = 5;
// const int echoPin = 18;
// const int buzzerPin = 2;  // Define buzzer pin

// // Wi-Fi credentials
// const char* ssid = "Mellow";       // Replace with your network SSID
// const char* password = "vishwa1234";   // Replace with your network password

// // ThingSpeak settings
// unsigned long myChannelNumber = 2628777;  // Replace with your ThingSpeak channel number
// const char* myWriteAPIKey = "WZ6UGSI5FA8WCOW9";  // Replace with your ThingSpeak write API key

// WiFiClient client;

// // Define sound speed in cm/uS
// #define SOUND_SPEED 0.034
// #define CM_TO_INCH 0.393701

// long duration;
// float distanceCm;
// float distanceInch;

// void setup() {
//   Serial.begin(115200); // Starts the serial communication
//   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
//   pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//   pinMode(buzzerPin, OUTPUT); // Sets the buzzerPin as an Output

//   // Connect to Wi-Fi
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to Wi-Fi...");
//   }
//   Serial.println("Connected to Wi-Fi");

//   // Initialize ThingSpeak
//   ThingSpeak.begin(client);
// }

// void loop() {
//   // Clears the trigPin
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   // Sets the trigPin on HIGH state for 10 micro seconds
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
  
//   // Reads the echoPin, returns the sound wave travel time in microseconds
//   duration = pulseIn(echoPin, HIGH);
  
//   // Calculate the distance
//   distanceCm = duration * SOUND_SPEED / 2;
  
//   // Prints the distance in the Serial Monitor
//   Serial.print("Distance (cm): ");
//   Serial.println(distanceCm);
  
//   // Send data to ThingSpeak
//   ThingSpeak.setField(1, distanceCm);  // Update field 1 with the distance value
//   int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//   if(x == 200){
//     Serial.println("Data sent to ThingSpeak successfully");
//   } else {
//     Serial.println("Error sending data to ThingSpeak");
//   }
  
//   // Activate buzzer if distance is less than 30 cm
//   if (distanceCm < 30) {
//     digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
//   } else {
//     digitalWrite(buzzerPin, LOW);   // Turn off buzzer
//   }
  
//   delay(1000);  // Wait for a second before the next reading
// }
