#define BLYNK_PRINT Serial
#include <WiFi.h>
#define BLYNK_TEMPLATE_ID "TMPL5M-ZwX-Z1"
#define BLYNK_TEMPLATE_NAME "AquaEdNBS"
#define BLYNK_AUTH_TOKEN "PnP2ZLCNgfOy7Xsd6Yit9PqDGmvktB1U"
#include <BlynkSimpleWifi.h>

char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = "Ayo_EXT";
const char* password = "Capsam2005.";

const int liquidSensorPin = 7;
const int pumpRelayPin = 2;

int pumpState = LOW;

void updatePumpState() {
  int liquidStatus = digitalRead(liquidSensorPin);

  if (liquidStatus == HIGH) {
    pumpState = LOW;  // Force OFF
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V2, HIGH);
    Serial.println("Liquid Detected! Pump OFF.");
  } else {
    Blynk.virtualWrite(V2, 0);
  }

  digitalWrite(pumpRelayPin, pumpState == HIGH ? LOW : HIGH);
}

// Handle Blynk switch
BLYNK_WRITE(V0) {
  pumpState = param.asInt();
  updatePumpState();  // Let sensor override if necessary
}

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

void loop() {
  Blynk.run();
  updatePumpState();
  delay(500);  // Optional: reduce if rapid sensor polling needed
}
