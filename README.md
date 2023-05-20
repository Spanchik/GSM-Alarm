# GSM Alarm System

This code implements an alarm system using a GSM module, motion sensor, LED and buzzer. The system can be armed or disarmed using SMS commands and sends SMS notifications for various events such as motion detection and low battery.

## Hardware requirements

- Arduino board (I used NodeMcu)
- GSM module (I used SIM800L)
- Motion Sensor
- LED
- Buzzer
- Resistors and jumper wires

## Circuit connections

1. Connect the GSM module to the Arduino board using RX and TX pins.
2. Connect the motion sensor, buzzer, and LED to the appropriate pins on the Arduino board.
3. Modify the `adminNumbers` array in the code to include the phone numbers of the administrators who will receive SMS notifications.

## Customize

1. Install the required libraries:
     - SoftwareSerial library (part of the Arduino IDE)

2. Connect the Arduino board to the computer.

3. Upload the code to the Arduino board.

4. Open the Serial Monitor in the Arduino IDE to view system status and debug information.

## Usage

1. Initialization:
     - After power on, the system is initialized by configuring the GSM module, setting up SMS notifications, and sending an initialization SMS to the administrator's phone number.

2. Turning the alarm on and off:
     - To arm, send an SMS with the text “On” to the GSM module phone number.
     - To turn off the alarm, send an SMS with the text "Off" to the GSM module phone number.

3. Checking the battery level:
     - To check the battery level, send an SMS with the text "Battery" to the phone number of the GSM module.
     - System will reply SMS with current battery level.

4. Motion detection:
     - When the alarm is enabled and the sensor detects motion, the system will trigger the alarm by turning on the LED and sending SMS notification.

5. Low battery notification:
     - The system periodically checks the battery level (by default, every 12 hours).
     - If the battery level is critically low (below 10%), the system sends an SMS notification to recharge the device.

## Setting

- Administrator phone numbers:
    - Add or change admin phone numbers in the `adminNumbers` array.
    - The system sends SMS alerts to all administrator's phone numbers.

- Pin configuration:
    - Change the pin assignment in the code according to your hardware connections.

- Deadlines:
    - Set the `waitTime` and `checkInterval` variables to change the response timeout and battery check interval respectively.

## Troubleshooting

- If the system does not respond to SMS commands, check the GSM module communication settings (baud rate) and make sure the connection with the Arduino is correct.
- If the motion sensor or other components are not working properly, check the wiring connections and pin assignments.
- If there are issues with SMS notifications, make sure the admin numbers are correctly set in the `adminNumbers` array and that the GSM module has a valid SIM card with sufficient credit.

