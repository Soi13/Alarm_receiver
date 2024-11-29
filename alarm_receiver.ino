#include <WiFi.h>

#define wifi_ssid "Soi13"
#define wifi_password ""

#define mqtt_server "10.0.0.4"
#define mqtt_user "mqtt_user"
#define mqtt_password ""

const int ledPin = 10;
const int buzzerPin = 8;

//This is for simulating of temporary lost WiFi connection.
//unsigned long startTime = millis();
//unsigned long disconnectAfter = 10000; // Disconnect after 10 seconds

//Blinking by red if WiFi connection have been lost.
void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(300);
  }
}

void reconnectWiFi() {
  WiFi.disconnect();
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.println("Reconnecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blinkLED(1);
    delay(1000);
  }
  Serial.println("\nReconnected to Wi-Fi!");
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost!");
    blinkLED(3);
    reconnectWiFi();
  } else {
    digitalWrite(ledPin, LOW);
  }

  //Test code block for simulating of temporary WiFi connection.
  /*if (millis() - startTime > disconnectAfter) {
    WiFi.disconnect();
    Serial.println("Simulated Wi-Fi disconnection.");
  }*/

}
