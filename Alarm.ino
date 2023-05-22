#include <SoftwareSerial.h>

SoftwareSerial gsmModule(D7, D8);  // RX=D7, TX=D8 pins for GSM module

const int motionPin = D3; // Motion sensor pin, connected to D3
const int buzzerPin = D1; // Buzzer pin, connected to D1
const int ledPin = D0; // LED pin, connected to D0
const unsigned long waitTime = 5000;  // Set the maximum waiting time for response
const unsigned long checkInterval = 12 * 60 * 60 * 1000; // Battery level check interval of 12 hours
unsigned long lastBatteryCheckTime = 0;

bool isArmed = false; // Alarm status
bool isMotionDetected = false; // Motion sensor status

String adminNumbers[] = {"+380123456789"};   // Record administrator phone numbers, you can add more than one


// Function for sending SMS messages
void sendSMS(String message) {
  // Iterate through each administrator number to send the SMS message to all administrators.
  for (int i = 0; i < sizeof(adminNumbers) / sizeof(adminNumbers[0]); i++) {
    gsmModule.println("AT+CMGS=\"" + adminNumbers[i] + "\""); // Send command to set the SMS recipient
    delay(1000); // Delay 1 second for the response
    gsmModule.println(message); // Send the text of the SMS message
    delay(1000);
    gsmModule.println((char)26); // Send Ctrl+Z character to end the SMS message
    delay(100);
    while (gsmModule.available()) {
      Serial.write(gsmModule.read()); // Output the response from the GSM module to Serial Monitor
    }
  }
}


//Initializes the system and sets up the necessary configurations.
void setup() {
  Serial.begin(9600); // Initialize the Serial communication
  delay(1000); // Delay 1 second for stability
  
  pinMode(motionPin, INPUT); // Set motionPin as input
  pinMode(ledPin, OUTPUT); // Set ledPin as output
  pinMode(buzzerPin, OUTPUT); // Set buzzerPin as output
  
  gsmModule.begin(9600); // Initialize the GSM module communication
  delay(1000);
  
  gsmModule.println("AT"); // Send AT command to the GSM module
  delay(1000); 
  while (gsmModule.available()) {
    Serial.write(gsmModule.read()); // Print the response from the GSM module to Serial Monitor
  }
  
  gsmModule.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000); 
  while (gsmModule.available()) {
    Serial.write(gsmModule.read()); 
  }
  
  gsmModule.println("AT+CNMI=1,2,0,0,0"); // Set up SMS notification mode
  delay(1000); 
  while (gsmModule.available()) {
    Serial.write(gsmModule.read());
  }

   // Delete all SMS messages
  gsmModule.println("AT+CMGD=1,4"); // Delete all messages in all storage locations
  delay(1000);
  while (gsmModule.available()) {
    Serial.write(gsmModule.read());
  }

  sendSMS("Alarm system initialized. Enter 'On' to arm the alarm, 'Off' to disarm, and 'Battery' to check battery status.");
  // Send an SMS to notify the administrators about the initialization and provide usage instructions.
}


void loop() {
  
  checkInput(); // check for user input and process commands
  checkMotionSensor(); // check motion sensor and send SMS if motion detected
  // check battery level every 12 hours and send SMS if critically lowOff
  unsigned long currentTime = millis();
  if (currentTime - lastBatteryCheckTime >= checkInterval) {
    checkBattery();
    lastBatteryCheckTime = currentTime;
  }

}


//Checks for incoming SMS messages and processes the commands.
void checkInput() {
  // Check if there is data available to read from the GSM module
  if (gsmModule.available()) {
  String receivedData = gsmModule.readStringUntil('\n'); // Read the data from the GSM module

    if (receivedData.startsWith("+CMT:")) {
      // Extract the sender number from the receivedData string
      int firstIndex = receivedData.indexOf('"');
      int secondIndex = receivedData.indexOf('"', firstIndex + 1);
      String senderNumber = receivedData.substring(firstIndex + 1, secondIndex);
      senderNumber.trim(); // Remove leading/trailing whitespaces
      Serial.println("Admin number found: " + senderNumber);

      // Read the SMS text
      String sms = gsmModule.readStringUntil('\n');
      Serial.println("SMS text: " + sms);
      sms.trim();

      // Check if the sender number is in the list of adminNumbers
      bool isAdmin = false;
      for (int i = 0; i < sizeof(adminNumbers) / sizeof(adminNumbers[0]); i++) {
        if (senderNumber == adminNumbers[i]) {
          isAdmin = true;
          break;
        }
      }

      if (isAdmin) {
        if (!sms.isEmpty()) {
          // Process the SMS command based on its content
          if (sms.indexOf("On") != -1) {
            armAlarm(); // Turn on the alarm
          } else if (sms.indexOf("Off") != -1) {
            disarmAlarm(); // Turn off the alarm
          } else if (sms.indexOf("Battery") != -1) {
            // Get the battery level and send it as an SMS response
            int batteryLevel = getBatteryStatus();
            if (!isnan(batteryLevel)) {
              sendSMS("Battery level: " + String(batteryLevel) + " %");
            } else {
              sendSMS("Unable to retrieve battery level.");
            }
          } else {
            sendSMS("Invalid input. Please enter 'On', 'Off', or 'Battery'."); // Send an SMS with an invalid input message
          }
        }
      } else {
        sendSMS("Unauthorized sender. Phone: " + senderNumber); // Sending SMS that the number is not authorized
      }
    }
  }
}


//Arming function
void armAlarm() {
  isArmed = true;  // Set the alarm status to armed
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  sendSMS("Alarm armed");  // Send an SMS notification that the alarm is armed
}


//Disarming function
void disarmAlarm() {
  isArmed = false;  // Set the alarm status to disarmed
  digitalWrite(ledPin, LOW);  // Turn off the LED
  digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  isMotionDetected = false;  // Reset the motion sensor status
  sendSMS("Alarm disarmed");  // Send an SMS notification that the alarm is disarmed
}


// Checks the motion sensor and triggers the alarm if motion is detected
void checkMotionSensor() {
  // Check if the alarm is armed and the motion sensor is triggered
  if (isArmed && !isMotionDetected && digitalRead(motionPin) == HIGH) {
    // Motion sensor detected motion
    isMotionDetected = true;  // Set the motion sensor status to true
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    sendSMS("Alarm! Motion detected");  // Send an SMS notification for motion detection
  }
  // If motion is detected, blink the LED and the buzzer
  if (isMotionDetected) {
    digitalWrite(ledPin, !digitalRead(ledPin));  // Toggle the LED state
    delay(500);  
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));  // Toggle the buzzer state
    delay(100); 
  }
}


// Retrieves the battery status from the GSM module
int getBatteryStatus() {
  gsmModule.println("AT+CBC");  // Send the command to request battery status
  unsigned long currentTime = millis();  // Get the current time
  String batteryResponse = "";  // Variable to store the battery response
  while (millis() - currentTime < waitTime) {
    if (gsmModule.available()) {
      batteryResponse += gsmModule.readStringUntil('\n');  // Read the response from the GSM module
    }
  }
  if (batteryResponse.indexOf("+CBC:") >= 0) {
    // Extract the battery level from the response
    const int commaIndex1 = batteryResponse.indexOf(',');  // Find the first comma
    const int commaIndex2 = batteryResponse.indexOf(',', commaIndex1 + 1);  // Find the second comma
    const String batteryLevelStr = batteryResponse.substring(commaIndex1 + 1, commaIndex2);  // Extract the battery level string
    return batteryLevelStr.toInt();  // Convert the battery level string to an integer and return it
  } else {
    sendSMS("Unable to read battery level");  // Send an SMS notification if unable to read battery level
    return -1;  // Return -1 to indicate an error
  }
}


// Checks the critical battery level at a specified interval.
void checkBattery() {
    int batteryLvl = getBatteryStatus(); // Get the current battery level
    if (batteryLvl >= 0 && batteryLvl < 10) {
      // Battery level is critically low
      sendSMS("Battery level is critically low at " + String(batteryLvl) + " %. Please recharge the device.");
    } 
}
