#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3CPxHUy2w"
#define BLYNK_TEMPLATE_NAME "iot skill"
#define BLYNK_AUTH_TOKEN "7ge870OFntFW7vkBTHt2Jy_2snzA-kWC"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// IR sensor pins
#define IR_SENSOR1_PIN 4  // Pin connected to the first IR sensor (e.g., G4 on ESP32)
#define IR_SENSOR2_PIN 5  // Pin connected to the second IR sensor (e.g., G5 on ESP32)

// Define LED pins for parking spots
#define LED1_PIN 2  // LED for the first parking spot
#define LED2_PIN 15 // LED for the second parking spot

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";  // Type your WiFi name
char pass[] = "  ";  // Type your WiFi password

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));

  Blynk.begin(auth, ssid, pass);

  pinMode(IR_SENSOR1_PIN, INPUT);  // Initialize the first IR sensor pin as an input
  pinMode(IR_SENSOR2_PIN, INPUT);  // Initialize the second IR sensor pin as an input
  pinMode(LED1_PIN, OUTPUT);  // Initialize the first LED pin as an output
  pinMode(LED2_PIN, OUTPUT);  // Initialize the second LED pin as an output
  digitalWrite(LED1_PIN, LOW);  // Ensure the first LED is off initially
  digitalWrite(LED2_PIN, LOW);  // Ensure the second LED is off initially

  Serial.println(F("Smart parking system initialized."));
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

  // Read the IR sensor values
  int irSensor1Value = digitalRead(IR_SENSOR1_PIN);
  int irSensor2Value = digitalRead(IR_SENSOR2_PIN);

  // Check if the first parking spot is occupied
  if (irSensor1Value == LOW) {  // Assuming LOW indicates a car is present
    Serial.println(F("Parking spot 1 occupied"));
    digitalWrite(LED1_PIN, HIGH);  // Turn on the LED for the first spot
    Blynk.virtualWrite(V0, "Spot 1: Occupied");  // Send status to Blynk virtual pin V0
  } else {
    Serial.println(F("Parking spot 1 available"));
    digitalWrite(LED1_PIN, LOW);  // Turn off the LED for the first spot
    Blynk.virtualWrite(V0, "Spot 1: Available");  // Send status to Blynk virtual pin V0
  }

  // Check if the second parking spot is occupied
  if (irSensor2Value == LOW) {  // Assuming LOW indicates a car is present
    Serial.println(F("Parking spot 2 occupied"));
    digitalWrite(LED2_PIN, HIGH);  // Turn on the LED for the second spot
    Blynk.virtualWrite(V1, "Spot 2: Occupied");  // Send status to Blynk virtual pin V1
  } else {
    Serial.println(F("Parking spot 2 available"));
    digitalWrite(LED2_PIN, LOW);  // Turn off the LED for the second spot
    Blynk.virtualWrite(V1, "Spot 2: Available");  // Send status to Blynk virtual pin V1
  }

  delay(1000);  // Wait for 1 second before the next loop
}

// BLYNK_WRITE(V2) {  // Button Widget on Virtual Pin V2 to control LED1 manually
//   int buttonState = param.asInt();

//   Serial.print(F("Button state for Spot 1: "));
//   Serial.println(buttonState);

//   if (buttonState == 1) {  // If button is pressed
//     Serial.println(F("Manually turning on LED for Spot 1"));
//     digitalWrite(LED1_PIN, HIGH);  // Turn on the LED for the first spot
//   } else {
//     Serial.println(F("Manually turning off LED for Spot 1"));
//     digitalWrite(LED1_PIN, LOW);  // Turn off the LED for the first spot
//   }
// }

// BLYNK_WRITE(V3) {  // Button Widget on Virtual Pin V3 to control LED2 manually
//   int buttonState = param.asInt();

//   Serial.print(F("Button state for Spot 2: "));
//   Serial.println(buttonState);

//   if (buttonState == 1) {  // If button is pressed
//     Serial.println(F("Manually turning on LED for Spot 2"));
//     digitalWrite(LED2_PIN, HIGH);  // Turn on the LED for the second spot
//   } else {
//     Serial.println(F("Manually turning off LED for Spot 2"));
//     digitalWrite(LED2_PIN, LOW);  // Turn off the LED for the second spot
//   }
// }
