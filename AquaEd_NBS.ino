#define BLYNK_PRINT Serial  // Enable Serial Monitor Debugging
#include <WiFi.h>         // WiFi Library for Arduino UNO R4 WiFi
#define BLYNK_TEMPLATE_ID "TMPL5M-ZwX-Z1"
#define BLYNK_TEMPLATE_NAME "AquaEdNBS"
#define BLYNK_AUTH_TOKEN "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U"
#include <BlynkSimpleWifi.h>  // Correct library for WiFiS3 boards


// Blynk Authentication Token (Replace with your actual token)
char auth[] = "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U"; 

const char* ssid = "Ayo_EXT";      // Replace with your Wi-Fi SSID
const char* password = "Capsam2005.";  // Replace with your Wi-Fi Password

// Define switch pin (change if needed)
const int fill1 = 2;

// Define sensor pin
const int liquidSensorPin = 7; // Connect the sensor output to D7

// Blynk function to control the switch from the app
BLYNK_WRITE(V0) {  
  int pumpState = param.asInt(); // Read value from app (0 or 1)
    // Check sensor before turning ON the pump
  if (digitalRead(liquidSensorPin) == HIGH) {  
    digitalWrite(fill1, LOW); // Turn OFF the pump if liquid is detected
    Blynk.virtualWrite(V2, "OFF"); // Update status on Blynk
  } else {
    digitalWrite(fill1, pumpState); // Control the relay normally
    Blynk.virtualWrite(V2, pumpState ? "ON" : "OFF"); // Update Blynk status
  }
}

// Function to check the sensor and update Blynk
void checkLiquidSensor() {
  int liquidStatus = digitalRead(liquidSensorPin);

  if (liquidStatus == HIGH) {
    digitalWrite(fill1, LOW); // Turn OFF the relay if liquid is detected
    Blynk.virtualWrite(V2, "OFF"); // Update Blynk status
  } 

  Blynk.virtualWrite(V1, liquidStatus * 100); // Send sensor status to gauge (0-100%)
}

void setup() {
  Serial.begin(115200);
  pinMode(fill1, OUTPUT);
  pinMode(liquidSensorPin, INPUT); // Set sensor pin as input

  while (!Serial); // Wait for Serial Monitor
  
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print local IP

  Blynk.begin(auth, ssid, password); // Connect to Blynk
}

void loop() {
  Blynk.run(); // Keep Blynk connection active
  checkLiquidSensor(); // Keep checking sensor and updating Blynk
}