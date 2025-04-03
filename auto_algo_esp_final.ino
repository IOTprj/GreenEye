#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

// CSS for styling
String style = R"rawliteral(
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #f4f4f4;
      padding: 20px;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .container {
      max-width: 400px;
      background: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);
    }
    h1 {
      color: #007BFF;
      margin-bottom: 20px;
    }
    .button-container {
      display: flex;
      flex-wrap: wrap;
      justify-content: space-between;
    }
    button {
      background-color: #007BFF;
      color: white;
      border: none;
      padding: 15px;
      margin: 10px;
      border-radius: 5px;
      font-size: 16px;
      cursor: pointer;
      flex: 1 1 48%;
    }
    button:active {
      background-color: #0056b3;
    }
    .switch-button {
      width: 100%;
      border-radius: 25px;
      background-color: #28a745;
    }
    .switch-button:active {
      background-color: #1e7e34;
    }
    @media (max-width: 600px) {
      .container {
        width: 90%;
        padding: 15px;
      }
      button {
        font-size: 14px;
        padding: 12px;
      }
    }
  </style>
)rawliteral";

const char* ssid = "v2027";
const char* password = "123456789";
const char* apiKey = "ESFUKQMUM14K7MUA"; // Your ThingSpeak API Key
int c = 0;
void setup() {
  Serial.begin(115200); // Start Serial communication for debugging

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, []() {
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>" + style + "</head><body>";
    html += "<div class='container'>";
    html += "<h1>Auto Mode</h1>";
    html += "<p>ESP8266 IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<form action='/control' method='GET'>";
    html += "<button class='switch-button' name='action' value='switch_to_manual'>Switch to Manual Mode</button>";
    html += "</form></div></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/manual", HTTP_GET, []() {
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>" + style + "</head><body>";
    html += "<div class='container'>";
    html += "<h1>Manual Mode</h1>";
    html += "<form action='/control' method='POST'>";
    html += "<div class='button-container'>";
    html += "<button name='action' value='pump_on'>Turn Pump ON</button>";
    html += "<button name='action' value='pump_off'>Turn Pump OFF</button>";
    html += "<button name='action' value='buzzer_on'>Turn Buzzer ON</button>";
    html += "<button name='action' value='buzzer_off'>Turn Buzzer OFF</button>";
    html += "<button name='action' value='solenoid1_off'>Turn Solenoid1 OFF</button>";
    html += "<button name='action' value='solenoid1_on'>Turn Solenoid1 ON</button>";
    html += "<button name='action' value='solenoid2_off'>Turn Solenoid2 OFF</button>";
    html += "<button name='action' value='solenoid2_on'>Turn Solenoid2 ON</button>";
    html += "<button name='action' value='solenoid3_off'>Turn Solenoid3 OFF</button>";
    html += "<button name='action' value='solenoid3_on'>Turn Solenoid3 ON</button>";
    html += "</div></form>";
    html += "<form action='/control' method='GET'>";
    html += "<button class='switch-button' name='action' value='switch_to_auto'>Switch to Auto Mode</button>";
    html += "</form></div></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/control", HTTP_GET, []() {
    String action = server.arg("action");
    if (action == "switch_to_auto") {
      Serial.println("switch_to_auto");
      Serial.println("Switching to Auto Mode...");
      server.sendHeader("Location", "/");
      c = 0;
      server.send(302, "text/plain", "Switching to Auto Mode...");
      return;
    }
    if (action == "switch_to_manual") {
      Serial.println("switch_to_manual");
      Serial.println("Switching to Manual Mode...");
      server.sendHeader("Location", "/manual");
      c = 1;
      server.send(302, "text/plain", "Switching to Manual Mode...");
      return;
    }
    server.send(200, "text/html", "<h1>Action Sent</h1><br><a href='/manual'>Go Back</a>");
  });

  server.on("/control", HTTP_POST, []() {
    String action = server.arg("action");
    if (action == "pump_on") Serial.println("pump_on");
    if (action == "pump_off") Serial.println("pump_off");
    if (action == "buzzer_on") Serial.println("buzzer_on");
    if (action == "buzzer_off") Serial.println("buzzer_off");
    if (action == "solenoid1_off") Serial.println("solenoid1_off");
    if (action == "solenoid1_on") Serial.println("solenoid1_on");
    if (action == "solenoid2_off") Serial.println("solenoid2_off");
    if (action == "solenoid2_on") Serial.println("solenoid2_on");
    if (action == "solenoid3_off") Serial.println("solenoid3_off");
    if (action == "solenoid3_on") Serial.println("solenoid3_on");
    server.sendHeader("Location", "/manual");
    server.send(302, "text/plain", "Updating Manual Mode...");
  });

  server.begin();
}

void loop() {

  // Check if data is available from Arduino
  if (Serial.available() > 0 && c == 0) {
    String receivedData = Serial.readStringUntil('\n'); // Read data from Arduino
   receivedData.trim();
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

  // //it is used to hadle the webpages
   server.handleClient();
  if (Serial.available()) {
    String comm = Serial.readStringUntil('\n');
    comm.trim();
    if(comm == "count_1"){
  delay(1000); // Wait before the next check
    }
    else{
      delay(4000);
    }
}
}
