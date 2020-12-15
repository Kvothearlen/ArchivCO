//theiotprojects.com
#include <Ultrasonic.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <SPIFFS.h>

// Instantiate trig and echo pins for ultrasonic sensors
Ultrasonic ultrasonic (4, 5);
// Replace with your network credentials
const char* ssid = "AndroidAPb78d";
const char* password = "loiclemoche";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getDistance() {
  // Read Distance
  float d = ultrasonic.read();
  if (isnan(d)) {
    Serial.println("Failed to read from HC-SR04 sensor!");
    return "";
  }
  else {
    Serial.println(d);
    return String(d);
  }
}
void setup () {
  // Serial port for debugging purposes
  Serial.begin (115200);

  // Initialize SPIFFS
  if (! SPIFFS.begin ()) {
    Serial.println ("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for web page
  server.on ("/", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request-> send (SPIFFS, "/index.html");
  });
  server.on ("/distance", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request-> send_P (200, "text / plain", getDistance(). c_str ());
  });

  // start server
  server.begin ();
}
void loop() {
}
