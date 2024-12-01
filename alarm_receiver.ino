#include <WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Soi13"
#define wifi_password ""

#define mqtt_server "10.0.0.4"
#define mqtt_port 1883
#define mqtt_user "mqtt_user"
#define mqtt_password ""

#define alarm_state_topic "homeassistant/sensor/fire_alarm"

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 10; //GPIO10
const int ledPin1 = 9; //GPIO9
const int ledPin2 = 20; //GPIO20
const int buzzerPin = 8; //GPIO8

//This is for simulating of temporary lost WiFi connection.
//unsigned long startTime = millis();
//unsigned long disconnectAfter = 10000; // Disconnect after 10 seconds

//Blinking by red or yellow if WiFi or MQTT connection have been lost.
void blinkLED(int times, int pin) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
    delay(300);
  }
}

void reconnectWiFi() {
  WiFi.disconnect();
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.println("Reconnecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blinkLED(1, ledPin);
    delay(1000);
  }
  Serial.println("\nReconnected to Wi-Fi!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Payload: ");
  Serial.println(message);

  // Control buzzer based on the command
  if (message == "ON") {
    tone(buzzerPin, 1200);
    Serial.println("Buzzer ON");
  } else if (message == "OFF") {
    noTone(buzzerPin);
    Serial.println("Buzzer OFF");
  } else {
    Serial.println("Unknown command");
  }
}

void reconnect_MQTT_broker() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("XIAO_ESP32C3_Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(alarm_state_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);

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

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && client.connected()){
      digitalWrite(ledPin2, HIGH);
  } else {
    digitalWrite(ledPin2, LOW);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost!");
    blinkLED(3, ledPin);
    reconnectWiFi();
  } else {
    digitalWrite(ledPin, LOW);
  }

  if (!client.connected()) {
    blinkLED(3, ledPin1);
    reconnect_MQTT_broker();
  }

   client.loop();

  //Test code block for simulating of temporary WiFi connection.
  /*if (millis() - startTime > disconnectAfter) {
    WiFi.disconnect();
    Serial.println("Simulated Wi-Fi disconnection.");
  }*/

}
