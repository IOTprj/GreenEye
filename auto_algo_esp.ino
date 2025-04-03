#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "v2027";
const char* password = "123456789";
const char* apiKey = "ESFUKQMUM14K7MUA"; // Your ThingSpeak API Key

void setup() {
  Serial.begin(115200); // Start Serial communication for debugging

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  // Check if data is available from Arduino
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n'); // Read data from Arduino

    // Parse the received data (split by commas)
    float field1, field2;
    int field3, field4, field5, field6, field7, field8;
    sscanf(receivedData.c_str(), "%f,%f,%d,%d,%d,%d,%d,%d", &field1, &field2, &field3, &field4, &field5, &field6, &field7, &field8);

    // Print received data for debugging
    Serial.print("Received Data: ");
    Serial.print(field1);
    Serial.print(", ");
    Serial.print(field2);
    Serial.print(", ");
    Serial.print(field3);
    Serial.print(", ");
    Serial.print(field4);
    Serial.print(", ");
    Serial.print(field5);
    Serial.print(", ");
    Serial.print(field6);
    Serial.print(", ");
    Serial.print(field7);
    Serial.print(", ");
    Serial.print(field8);
    Serial.println(" "); 

    // Send data to ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + String(apiKey) +
                   "&field1=" + String(field1) +
                   "&field2=" + String(field2) +
                   "&field3=" + String(field3) +
                   "&field4=" + String(field4) +
                   "&field5=" + String(field5) +
                   "&field6=" + String(field6) +
                   "&field7=" + String(field7) +
                   "&field8=" + String(field8);

      http.begin(client,url);
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("Data sent to ThingSpeak successfully");
      } else {
        Serial.println("Error sending data to ThingSpeak");
      }
      http.end();
    }
  }

  delay(5000); // Wait before the next check
}
