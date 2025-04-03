#define PUMP_PIN 8       // Water Pump control pin
#define BUZZER_PIN 9     // Buzzer pin (digital pin 9)
#define SOLENOID_PIN 5   // Solenoid Valve control pin

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SOLENOID_PIN, OUTPUT);

  // Initialize all components as OFF initially
  digitalWrite(PUMP_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(SOLENOID_PIN, LOW);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
   Serial.println(command);
    if (command == "pump_on") {
      digitalWrite(PUMP_PIN, LOW); // Turn on pump
    } else if (command == "pump_off") {
      digitalWrite(PUMP_PIN, HIGH); // Turn off pump
    } else if (command == "buzzer_on") {
      digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
    } else if (command == "buzzer_off") {
      digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
    } else if (command == "solenoid_on") {
      digitalWrite(SOLENOID_PIN, HIGH); // Open solenoid valve
    } else if (command == "solenoid_off") {
      digitalWrite(SOLENOID_PIN, LOW); // Close solenoid valve
    }
    else if(command == "switch_to_auto"){
      digitalWrite(BUZZER_PIN, HIGH);
    }
    else if(command == "switch_to_manual"){
      digitalWrite(BUZZER_PIN, LOW);
    }  
  }
}
