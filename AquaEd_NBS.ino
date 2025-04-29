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

// Fill pump relays
const int fillRelayPin = 2;   // Main fill pump
const int fillRelayPin1 = 3;  // Auxiliary fill pump 1
const int fillRelayPin2 = 4;  // Auxiliary fill pump 2

// Drain pump relays
const int drainRelayPin1 = 5;
const int drainRelayPin2 = 6;

int fillPumpState = LOW;

// Non-blocking timers
bool fillRelaysActive = false;
unsigned long fillRelaysStartTime = 0;

bool drainRelaysActive = false;
unsigned long drainRelaysStartTime = 0;

void updateFillPumpState() {
  int liquidStatus = digitalRead(liquidSensorPin);

  if (liquidStatus == HIGH && fillPumpState == HIGH) {
    // Liquid detected, turn off main fill pump and activate auxiliary relays
    fillPumpState = LOW;
    digitalWrite(fillRelayPin, HIGH);     // OFF (active LOW logic)
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V2, HIGH);
    Serial.println("Liquid Detected! Fill Pump OFF. Aux Relays ON for 6s.");

    digitalWrite(fillRelayPin1, LOW);     // ON
    digitalWrite(fillRelayPin2, LOW);     // ON
    Blynk.virtualWrite(V3, 255);          // Turn ON virtual LED for aux pumps

    fillRelaysActive = true;
    fillRelaysStartTime = millis();
  } else if (liquidStatus != HIGH) {
    // No liquid, allow control via Blynk
    Blynk.virtualWrite(V2, 0);
    digitalWrite(fillRelayPin, fillPumpState == HIGH ? LOW : HIGH);
  }
}

// Blynk switch to control fill pump (V0)
BLYNK_WRITE(V0) {
  fillPumpState = param.asInt();
  updateFillPumpState();
}

// Blynk switch to control drain pumps (V1)
BLYNK_WRITE(V1) {
  int drainCommand = param.asInt();
  if (drainCommand == 1 && !drainRelaysActive) {
    digitalWrite(drainRelayPin1, LOW);  // ON
    digitalWrite(drainRelayPin2, LOW);  // ON
    Serial.println("Drain pumps ON for 10 seconds");

    drainRelaysActive = true;
    drainRelaysStartTime = millis();
  }
}

void setup() {
  Serial.begin(115200);

  // Pin setup
  pinMode(liquidSensorPin, INPUT);
  pinMode(fillRelayPin, OUTPUT);
  pinMode(fillRelayPin1, OUTPUT);
  pinMode(fillRelayPin2, OUTPUT);
  pinMode(drainRelayPin1, OUTPUT);
  pinMode(drainRelayPin2, OUTPUT);

  // All relays OFF initially (active LOW)
  digitalWrite(fillRelayPin, HIGH);
  digitalWrite(fillRelayPin1, HIGH);
  digitalWrite(fillRelayPin2, HIGH);
  digitalWrite(drainRelayPin1, HIGH);
  digitalWrite(drainRelayPin2, HIGH);

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
  updateFillPumpState();

  // Turn off fill auxiliary relays after 6 seconds
  if (fillRelaysActive && millis() - fillRelaysStartTime >= 6000) {
    digitalWrite(fillRelayPin1, HIGH);
    digitalWrite(fillRelayPin2, HIGH);
    Blynk.virtualWrite(V3, 0);  // Turn OFF virtual LED for aux pumps
    fillRelaysActive = false;
    Serial.println("Fill auxiliary relays OFF after 6 seconds");
  }

  // Turn off drain relays after 10 seconds
  if (drainRelaysActive && millis() - drainRelaysStartTime >= 10000) {
    digitalWrite(drainRelayPin1, HIGH);
    digitalWrite(drainRelayPin2, HIGH);
    drainRelaysActive = false;
    Blynk.virtualWrite(V1, 0);  // Reset Blynk switch
    Serial.println("Drain pumps OFF after 10 seconds");
  }

  delay(100);  // Optional delay
}
