#include <DHT.h>
#include <LiquidCrystal_I2C.h>  // Include the LCD library

// Pin definitions
#define DHTPIN 2         // Pin for DHT11 sensor
#define DHTTYPE DHT11    // Define DHT type
#define MOISTURE_PIN1 A0 // Moisture Sensor 1 pin
#define MOISTURE_PIN2 A1 // Moisture Sensor 2 pin
#define MOISTURE_PIN3 A2 // Moisture Sensor 3 pin
#define RAIN_PIN 3       // Rain Sensor pin
#define LASER_PIN 4      // Laser sensor pin
#define BUZZER_PIN 9     // Buzzer pin (digital pin 9)
#define SOLENOID1_PIN 5  // Solenoid Valve 1
#define SOLENOID2_PIN 6  // Solenoid Valve 2
#define SOLENOID3_PIN 7  // Solenoid Valve 3
#define PUMP_PIN 8       // Water Pump control pin

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD (change the I2C address if necessary)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and screen size if required

// Define variables
float temperature, humidity;
int moisture1, moisture2, moisture3;
int rainStatus, laserStatus;
int pumpStatus = 0;  // 0 = OFF, 1 = ON

void setup() {
  Serial.begin(115200);  // Initialize serial communication for ESP8266
  lcd.backlight();

  // Initialize DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.begin(16, 2);  // 16 columns, 2 rows
  lcd.clear();

  // Set solenoid valves, pump, and sensor pins as outputs/inputs
  pinMode(SOLENOID1_PIN, OUTPUT);
  pinMode(SOLENOID2_PIN, OUTPUT);
  pinMode(SOLENOID3_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(LASER_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);  // Buzzer pin as output

  // Initialize all valves and pump as OFF initially
  digitalWrite(SOLENOID1_PIN, LOW);
  digitalWrite(SOLENOID2_PIN, LOW);
  digitalWrite(SOLENOID3_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);  // Buzzer OFF initially
}

void loop() {
  // Read the rain sensor first to decide whether to stop watering
  rainStatus = digitalRead(RAIN_PIN);

  // Read moisture values
  moisture1 = analogRead(MOISTURE_PIN1);
  moisture2 = analogRead(MOISTURE_PIN2);
  moisture3 = analogRead(MOISTURE_PIN3);

  // Read temperature and humidity from DHT11
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check if any reading failed
  if (isnan(temperature) || isnan(humidity)) {
    // Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Clear LCD and print moisture readings first
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("M1:");
  lcd.print(moisture1);
  lcd.print(" M2:");
  lcd.print(moisture2);
  lcd.setCursor(0, 1);
  lcd.print("M3:");
  lcd.print(moisture3);

  delay(2000);  // Delay to allow the data to be visible

  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("M3:");
  //lcd.print(moisture3);

 // delay(2000);  // Delay to allow the data to be visible

  // Now print temperature and humidity readings
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000);  // Delay to allow the data to be visible

  // Print rain status last
  lcd.clear();
  lcd.setCursor(0, 0);
  if (rainStatus == LOW) {
    lcd.print("Rain Detected!");
  } else {
    lcd.print("No Rain");
  }
  delay(2000);  // Delay to allow the data to be visible
  
  lcd.clear();
  lcd.setCursor(0, 0);
  if (laserStatus == HIGH){
    lcd.print("Intruder");
    lcd.setCursor(0, 1);
    lcd.print("detected!");

    }else {
     lcd.setCursor(0, 0); 
     lcd.print(" NO Intruder");
     lcd.setCursor(0, 1);
     lcd.print(" detected!");
    }




  // Control solenoids based on moisture levels
  if (moisture1 < 500) {
    digitalWrite(SOLENOID1_PIN, HIGH);
    //pumpStatus = 1;
  } else {
    digitalWrite(SOLENOID1_PIN, LOW);

  }

  if (moisture2 < 500) {
    digitalWrite(SOLENOID2_PIN, HIGH);
   // pumpStatus = 1;
  } else {
    digitalWrite(SOLENOID2_PIN, LOW);
    
  }

  if (moisture3 < 500) {
    digitalWrite(SOLENOID3_PIN, HIGH);
   // pumpStatus = 1;
  } else {
    digitalWrite(SOLENOID3_PIN, LOW);
 
  }




  // Determine pump status based on moisture readings
  if (moisture1 > 600 || moisture2 > 600 || moisture3 > 600) {
    pumpStatus = 0;  // Turn on pump if any sensor reads low
  } else {
    pumpStatus = 1;  // Turn off pump if all are sufficiently wet
  }



//  if (moisture1 >600 || moisture2 > 600 || moisture3 > 600) {
//     pumpStatus = 1;  // Turn on pump if any sensor reads low
//   } else {
//     pumpStatus = 0;  // Turn off pump if all are sufficiently wet
//   }

  // Anti-theft protection: Activate buzzer if laser is triggered
  laserStatus = digitalRead(LASER_PIN);
  if (laserStatus == HIGH) {  // Assuming LOW indicates a laser break (intruder detected)
    digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
    // Serial.println("Intruder detected! Buzzer ON.");
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer when laser is intact
  }

  // Send data to ESP8266 (Arduino Mega to ESP8266 via Serial)
  // Serial.print("temperature: ");
  // Serial.print(temperature);
  // Serial.print(",");
  // Serial.print("humidity: ");
  // Serial.print(humidity);
  // Serial.print(",");
  // Serial.print("moisture1: ");
  // Serial.print(moisture1);
  // Serial.print(",");
  // Serial.print("moisture2: ");
  // Serial.print(moisture2);
  // Serial.print(",");
  // Serial.print("moisture3: ");
  // Serial.print(moisture3);
  // Serial.print(",");
  // Serial.print("rainStatus: ");
  // Serial.print(rainStatus);
  // Serial.print(",");
  // Serial.print("laserStatus: ");
  // Serial.print(laserStatus);
  // Serial.print(",");
  // Serial.print("pumpStatus: ");
  // Serial.println(pumpStatus); // Send pump status (1 or 0)


  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(moisture1);
  Serial.print(",");
  Serial.print(moisture2);
  Serial.print(",");
  Serial.print(moisture3);
  Serial.print(",");
  Serial.print(rainStatus);
  Serial.print(",");
  Serial.print(laserStatus);
  Serial.print(",");
  Serial.println(pumpStatus); // Send pump status (1 or 0)

  if(rainStatus == 0) pumpStatus = 1;
  // Control the water pump based on moisture
  if (pumpStatus == 1) {
    digitalWrite(PUMP_PIN, HIGH);  // Turn off water pump
  } else {
    digitalWrite(PUMP_PIN, LOW);   // Turn on water pump
  }






  delay(2000); // Delay between readings
}