# GSM Alarm System Project

This project demonstrates an alarm system implemented using an Arduino board and a GSM module. The system detects motion using a motion sensor and sends SMS notifications to predefined administrator phone numbers. The administrators can remotely arm and disarm the alarm system, as well as check the battery level of the device.

## Hardware requirements

- Arduino board (I used NodeMcu)
- GSM module (I used SIM800L)
- Motion Sensor
- LED
- Buzzer
- Resistors and jumper wires

## Functionality Overview

The alarm system project includes the following main functionalities:

1. **Arming and Disarming**: Administrators can remotely arm and disarm the alarm system by sending SMS commands to the GSM module. The alarm system will acknowledge the status change via SMS notifications.
2. **Motion Detection**: The system continuously monitors the motion sensor. If motion is detected while the alarm is armed, an SMS notification is sent to the administrators.
3. **Battery Level Monitoring**: The system periodically checks the battery level and sends an SMS notification to the administrators if the battery level falls below a critical threshold.

## Setup Instructions

Follow these steps to set up the alarm system project:

1. **Hardware Connection**: Connect the Arduino board, GSM module, motion sensor, buzzer, and LED according to the provided pin configuration.
2. **Library Installation**: Make sure you have the required libraries installed. This project uses the SoftwareSerial library, which is typically included with the Arduino IDE.
3. **Configuration**: Update the administrator phone numbers in the adminNumbers array to the desired phone numbers that will receive the alarm notifications.
4. **Upload Code**: Upload the alarm_system.ino sketch to the Arduino board using the Arduino IDE.
5. **Serial Monitor**: Open the Serial Monitor in the Arduino IDE to view the system initialization process and debug messages.
6. **Testing**: Test the alarm system by sending SMS commands to arm, disarm, and trigger the motion detection.

## Usage

◉ **Initialization**:
     - After power on, the system is initialized by configuring the GSM module, setting up SMS notifications, and sending an initialization SMS to the administrator's phone number.

◉ **Turning the alarm on and off**:
     - To arm, send an SMS with the text “On” to the GSM module phone number.
     - To turn off the alarm, send an SMS with the text "Off" to the GSM module phone number.

◉ **Checking the battery level**:
     - To check the battery level, send an SMS with the text "Battery" to the phone number of the GSM module.
     - System will reply SMS with current battery level.

◉ **Motion detection**:
     - When the alarm is enabled and the sensor detects motion, the system will trigger the alarm by turning on the LED and sending SMS notification.

◉ **Low battery notification**:
     - The system periodically checks the battery level (by default, every 12 hours).
     - If the battery level is critically low (below 10%), the system sends an SMS notification to recharge the device.


## Troubleshooting

If you encounter any issues with the alarm system project, consider the following troubleshooting steps:

1. **Serial Monitor**: Check the Serial Monitor in the Arduino IDE for any error messages or unexpected behavior.

2. **Hardware Connections**: Verify that all components are correctly connected according to the provided pin configuration.

3. **Library Dependencies**: Ensure that all required libraries, such as SoftwareSerial, are properly installed in your Arduino IDE.

4. **GSM Module Configuration**: Review the configuration commands sent to the GSM module during setup. Check the GSM module documentation for any specific configuration requirements.

5. **Battery Level Reading**: If the battery level cannot be read, ensure that the GSM module is properly powered and connected to the battery.

6. **Administrator Numbers**: Double-check the administrator phone numbers in the adminNumbers array to ensure they are entered correctly.

If the issue persists, refer to the Arduino and GSM module documentation for further troubleshooting steps.

## Conclusion

The alarm system project provides a basic implementation of an Arduino-based alarm system with GSM module integration. By following the setup
