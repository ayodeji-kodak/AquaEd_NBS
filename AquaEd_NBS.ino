#define BLYNK_PRINT Serial  // Enable Serial Monitor Debugging
#include <WiFiS3.h>         // WiFi Library for Arduino UNO R4 WiFi
#define BLYNK_TEMPLATE_ID "TMPL5M-ZwX-Z1"
#define BLYNK_TEMPLATE_NAME "AquaEdNBS"
#define BLYNK_AUTH_TOKEN "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U"
#include <BlynkSimpleWiFiS3.h>  // Correct library for WiFiS3 boards


// Blynk Authentication Token (Replace with your actual token)
char auth[] = "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U"; 

const char* ssid = "Ayo_EXT";      // Replace with your Wi-Fi SSID
const char* password = "Capsam2005.";  // Replace with your Wi-Fi Password

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor
  
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print local IP

  Blynk.begin(auth, ssid, pass); // Connect to Blynk
}

void loop() {
  Blynk.run(); // Keep Blynk connection active
}