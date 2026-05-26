#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "PRATEEK";
const char* password = "PRATEEL";

void setup() {

  Serial.begin(115200);

  // WiFi Connect
  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Show IP
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // OTA Start
  ArduinoOTA.setHostname("ESP32-OTA");

  ArduinoOTA.begin();

  Serial.println("OTA Ready");
}

void loop() {

  // OTA Handle
  ArduinoOTA.handle();
}