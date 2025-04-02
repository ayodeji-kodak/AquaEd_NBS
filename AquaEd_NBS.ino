#define BLYNK_PRINT Serial  // Enable Serial Monitor Debugging
#include <WiFi.h>           // WiFi Library for Arduino UNO R4 WiFi
#define BLYNK_TEMPLATE_ID "TMPL5M-ZwX-Z1"
#define BLYNK_TEMPLATE_NAME "AquaEdNBS"
#define BLYNK_AUTH_TOKEN "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U"
#include <BlynkSimpleWifi.h>  // Correct library for WiFiS3 boards


// Blynk Authentication Token (Replace with your actual token)
char auth[] = "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U";

const char* ssid = "Ayo_EXT";          // Replace with your Wi-Fi SSID
const char* password = "Capsam2005.";  // Replace with your Wi-Fi Password

// Define sensor and relay pins
const int liquidSensorPin = 7; // Liquid sensor connected to D7
const int pumpRelayPin = 2;    // Pump relay connected to D2

int pumpState = LOW; // Default: Pump is OFF

// Blynk function to control pump from switch (V0)
BLYNK_WRITE(V0) {
  pumpState = param.asInt();  // Read switch state from Blynk (0 or 1)
  
  int liquidStatus = digitalRead(liquidSensorPin); // Read liquid sensor
  
  if (liquidStatus == HIGH) { 
    pumpState = LOW;  // Turn OFF pump if liquid detected
    Blynk.virtualWrite(V0, 0);  // Flip switch in Blynk app to OFF
    Serial.println("Pump forced OFF due to liquid detection!");
  }

  digitalWrite(pumpRelayPin, pumpState == HIGH ? LOW : HIGH); // Control pump relay
}

// Setup function
void setup() {
  Serial.begin(115200);

  pinMode(liquidSensorPin, INPUT);
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, HIGH);  // Start with pump OFF

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  Blynk.begin(auth, ssid, password);
}

// Main loop
void loop() {
  Blynk.run();

  int liquidStatus = digitalRead(liquidSensorPin); // Check sensor
  
  if (liquidStatus == HIGH) {
    Serial.println("Liquid Detected! Turning OFF pump.");
    pumpState = LOW;  // Ensure pump is OFF
    digitalWrite(pumpRelayPin, HIGH);
    Blynk.virtualWrite(V0, 0);  // Flip switch in Blynk app to OFF
  } else {
    Serial.println("No Liquid Detected.");
  }

  delay(500);
}