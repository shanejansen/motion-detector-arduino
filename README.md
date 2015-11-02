# Motion Detector Arduino #

### Overview ###

This is the Arduino microcontroller code for an infrared motion sensor connected to an Android application.
When motion is detected, the microcontroller communicates to a running Android application via Bluetooth
to play a sound and show a notification. The notification is dismissed and the microcontroller
is ready to detect the next movement. The alarm can be disabled for 1 minute from the application
and if the connection is lost during use, a different sound will play.

### Requirements ###

When building the Wi-Fi portion of this project, you must include the "Arduino ESP8266 Helper" library
which can be found here: https://github.com/ShaneJansen/ArduinoEsp8266Helper

### Notes ###

To see a working example, list of parts, and video demonstration visit: http://sjjapps.com/
