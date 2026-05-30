#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html",
              "<!DOCTYPE html>"
              "<html>"
              "<head>"
              "<title>ESP32 Web Server</title>"
              "<style>"
              "body{display:flex;justify-content:center;align-items:center;height:100vh;"
              "font-family:Arial;background:#f0f8ff;}"
              "h1{color:green;font-size:50px;}"
              "</style>"
              "</head>"
              "<body>"
              "<h1>🍀 Good Luck! 🍀</h1>"
              "</body>"
              "</html>");
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}