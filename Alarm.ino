#include <SoftwareSerial.h>

SoftwareSerial gsmModule(D7, D8);  // RX=D7, TX=D8 pins for GSM module

const int motionPin = D3; // Motion sensor pin, connected to D3
const int buzzerPin = D1; // Buzzer pin, connected to D1
const int ledPin = D0; // LED pin, connected to D0
const unsigned long BATTERY_CHECK_INTERVAL = 6 * 60 * 60 * 1000; // 6 hours in milliseconds
unsigned long lastBatteryCheckTime = 0; // Variable to store the timestamp of the last battery check


bool isArmed = false; // Alarm status
bool isMotionDetected = false; // Motion sensor status  

String adminNumbers[] = {"+380681496848"};   // Record administrator phone numbers, you can add more than one


void sendSMS(String message) {
  // Send an SMS to each admin number
  for (int i = 0; i < sizeof(adminNumbers) / sizeof(adminNumbers[0]); i++) {
    if (isAdminNumber(adminNumbers[i])) {
      // Format the AT command to send an SMS
      String command = "AT+CMGS=\"" + adminNumbers[i] + "\"\r\n";
      // Send the command to the GSM module
      gsmModule.print(command);
      delay(1000);
      // Send the SMS message
      gsmModule.print(message);
      delay(100);
      // Send the Ctrl+Z character to indicate the end of the message
      gsmModule.print((char)26);
      delay(1000);
    }
  }
}


void setup() {
  // Configure the serial port for communication with the GSM module
  gsmModule.begin(9600);
  delay(1000);
  // Send the command to set the SMS text mode
  gsmModule.println("AT+CMGF=1\r\n"); // Set the text mode command
  delay(500);
  gsmModule.println("AT+CNMI=2,2,0,0,0\r\n");  // Set the notification for receiving a new SMS command
  delay(500);
  sendSMS("Alarm system initialized. Enter 'On' to arm the alarm and 'Off' to disarm.");
}


void loop() {
  checkMotionSensor(); // check motion sensor and send SMS if motion detected
  checkSMS(); // check for user input and process commands

  // Check battery level every 6 hours
  unsigned long currentTime = millis();
  if (currentTime - lastBatteryCheckTime >= BATTERY_CHECK_INTERVAL) {
    monitorBatteryLevel();
    lastBatteryCheckTime = currentTime;
  }

}


bool isAdminNumber(String sender) {
  // Check if the given sender is an admin number
  for (int i = 0; i < sizeof(adminNumbers) / sizeof(adminNumbers[0]); i++) {
    if (sender == adminNumbers[i]) {
      return true;  // Sender is an admin number
    }
  }
  return false;  // Sender is not an admin number
}


void checkSMS() {
  // Check for incoming SMS messages
  while (gsmModule.available()) {
    String message = gsmModule.readStringUntil('\n');
    message.trim();  // Remove leading and trailing whitespaces
    if (message.length() > 0) {
      // Check the sender of the message
      if (isAdminNumber(message)) {
        // Process the message from an admin
        processAdminCommand(message);
      } else {
        // Ignore messages from non-admins
      }
    }
  }
}


void processAdminCommand(String command) {
  // Process the admin command based on the received command string
  // Check if the command contains "On"
  if (command.indexOf("On") >= 0) {
    armAlarm();  // Call the armAlarm() function to arm the alarm
  }
  // Check if the command contains "Off"
  else if (command.indexOf("Off") >= 0) {
    disarmAlarm();  // Call the disarmAlarm() function to disarm the alarm
  }
  // Check if the command contains "Battery"
  else if (command.indexOf("Battery") >= 0) {
    checkBatteryStatus();  // Call the checkBatteryStatus() function to check the battery status
  }
  // Unknown command
  else {
    // Send an SMS with an error message indicating the unknown command
    sendSMS("Unknown command: " + command + " Please enter 'On', 'Off', or 'Battery'");
  }
}


void armAlarm() {
  isArmed = true;  // Set the alarm status to armed
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  sendSMS("Alarm armed");  // Send an SMS notification that the alarm is armed
 }


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
    delay(500);  // Delay for 500 milliseconds
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));  // Toggle the buzzer state
    delay(100);
  }
}


//Disarming function
void disarmAlarm() {
  isArmed = false;  // Set the alarm status to disarmed
  digitalWrite(ledPin, LOW);  // Turn off the LED
  digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  isMotionDetected = false;  // Reset the motion sensor status
  sendSMS("Alarm disarmed");  // Send an SMS notification that the alarm is disarmed
}

void checkBatteryStatus() {
  // Send AT command to check battery status
  gsmModule.println("AT+CBC\r\n");
  delay(1000);
  // Read the response from the GSM module
  String response = gsmModule.readStringUntil('\n');
  response.trim();
  // Check if the response contains battery information
  if (response.indexOf("+CBC:") >= 0) {
    // Find the comma indices
    int commaIndex1 = response.indexOf(",");
    int commaIndex2 = response.indexOf(",", commaIndex1 + 1);
    if (commaIndex1 >= 0 && commaIndex2 >= 0) {
      // Extract the battery status
      String batteryLevel = response.substring(commaIndex1 + 1, commaIndex2);
      // Send the battery status to the admin numbers
      sendSMS("Battery Status: " + batteryLevel + "%");
      return;
    }
  }
  // If battery status retrieval failed, send an error message
  sendSMS("Failed to retrieve battery status");
}

void monitorBatteryLevel() {
  // Send AT command to check battery status
  gsmModule.println("AT+CBC\r\n");
  delay(1000);
  // Read the response from the GSM module
  String response = gsmModule.readStringUntil('\n');
  response.trim();
  // Check if the response contains battery information
  if (response.indexOf("+CBC:") >= 0) {
    // Find the comma indices
    int commaIndex1 = response.indexOf(",");
    int commaIndex2 = response.indexOf(",", commaIndex1 + 1);
    if (commaIndex1 >= 0 && commaIndex2 >= 0) {
      // Extract the battery level
      String batteryLevelStr = response.substring(commaIndex2 + 1);
      int batteryLevel = batteryLevelStr.toInt();
      // Check if battery level is critical
      if (batteryLevel <= 10) {
        sendSMS("Battery level is critical: " + batteryLevelStr + "%");
      }
      return;
    }
  }
  // If battery status retrieval failed, send an error message
  sendSMS("Failed to retrieve battery status");
}
