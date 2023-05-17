# GSM Alarm System

This project implements a motion sensor alarm system using an Arduino board and a GSM module. The system detects motion using a motion sensor and sends SMS notifications to admin numbers when motion is detected. It supports arming and disarming of the alarm via SMS commands.

## Hardware Requirements

- Arduino board (e.g., NodeMcu)
- GSM module (e.g., SIM800L)
- Motion sensor
- Buzzer
- LED
- Resistors and jumper wires

## Setup and Configuration

1. Connect the GSM module to the Arduino board using RX and TX pins.
2. Connect the motion sensor, buzzer, and LED to the appropriate pins on the Arduino board.
3. Modify the `adminNumbers` array in the code to include the phone numbers of the administrators who will receive SMS notifications.
4. Upload the code to the Arduino board.

## Functionality

The code provides the following functionality:

- When the system is powered on, it initializes by sending an SMS to the admin numbers with an initialization message.
- The system checks the motion sensor periodically. If motion is detected and the alarm is armed, an SMS notification is sent to the admin numbers.
- The system listens for incoming SMS messages. If an admin number sends a command to arm or disarm the alarm, the system executes the corresponding action and sends an SMS confirmation.
- The system also provides a battery status check command. Sending the command "Battery" will trigger a check of the battery status, and an SMS notification will be sent to the admin numbers with the battery level.

## Usage

1. Power on the system.
2. The system will send an initialization SMS to the admin numbers.
3. To arm the alarm, send an SMS to the GSM module with the command "On". The system will arm the alarm and send an SMS confirmation.
4. When the alarm is armed, any motion detected by the motion sensor will trigger an SMS notification to the admin numbers.
5. To disarm the alarm, send an SMS with the command "Off". The system will disarm the alarm, turn off the LED and buzzer, and send an SMS confirmation.
6. To check the battery status, send an SMS with the command "Battery". The system will retrieve the battery status and send an SMS notification with the battery level.

## Troubleshooting

- If the system is not responding to SMS commands, ensure that the GSM module is properly connected and powered.
- If the motion sensor is not detecting motion, check the wiring connections and adjust the sensitivity if necessary.
- If there are issues with SMS notifications, make sure the admin numbers are correctly set in the `adminNumbers` array and that the GSM module has a valid SIM card with sufficient credit.

