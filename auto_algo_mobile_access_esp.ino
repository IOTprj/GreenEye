#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "v2027";
const char* password = "123456789";

ESP8266WebServer server(80); // Web server running on port 80

void setup() {
  Serial.begin(115200); // Start Serial communication
  WiFi.begin(ssid, password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the IP address on the Serial Monitor

  // First Page (Auto Mode)
  server.on("/", HTTP_GET, [](){
    String html = "<html><body>";
    html += "<h1>Auto Mode</h1>";
    html += "<p>ESP8266 IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<form action='/control' method='GET'>";
    html += "<button name='action' value='switch_to_manual'>Switch to Manual Mode</button>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
  });

  // Second Page (Manual Mode)
  server.on("/manual", HTTP_GET, [](){
    String html = "<html><body>";
    html += "<h1>Manual Mode</h1>";
    html += "<form action='/control' method='POST'>";
    html += "<button name='action' value='pump_on'>Turn Pump ON</button><br><br>";
    html += "<button name='action' value='pump_off'>Turn Pump OFF</button><br><br>";
    html += "<button name='action' value='buzzer_on'>Turn Buzzer ON</button><br><br>";
    html += "<button name='action' value='buzzer_off'>Turn Buzzer OFF</button><br><br>";
    html += "<button name='action' value='solenoid_on'>Turn Solenoid ON</button><br><br>";
    html += "<button name='action' value='solenoid_off'>Turn Solenoid OFF</button><br><br>";
    html += "</form>";
    html += "<form action='/control' method='GET'>";
    html += "<button name='action' value='switch_to_auto'>Switch to Auto Mode</button>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
  });

  // Handle the control actions
  server.on("/control", HTTP_GET, [](){
    String action = server.arg("action");

    if (action == "switch_to_auto") {
      Serial.println("switch_to_auto");
      server.sendHeader("Location", "/");  // Redirect to Auto Mode page
      server.send(302, "text/plain", "Switching to Auto Mode...");
      return;
    }
    if (action == "switch_to_manual") {
      Serial.println("switch_to_manual");
      server.sendHeader("Location", "/manual");  // Redirect to Manual Mode page
      server.send(302, "text/plain", "Switching to Manual Mode...");
      return;
    }

    server.send(200, "text/html", "<h1>Action Sent</h1><br><a href='/manual'>Go Back</a>");
  });

  server.on("/control", HTTP_POST, [](){
    String action = server.arg("action");

    if (action == "pump_on") {
      Serial.println("pump_on");
    }
    if (action == "pump_off") {
      Serial.println("pump_off");
    }
    if (action == "buzzer_on") {
      Serial.println("buzzer_on");
    }
    if (action == "buzzer_off") {
      Serial.println("buzzer_off");
    }
    if (action == "solenoid_on") {
      Serial.println("solenoid_on");
    }
    if (action == "solenoid_off") {
      Serial.println("solenoid_off");
    }  
  });

  server.begin();
}

void loop() {
  server.handleClient(); // Handle web server requests
}
