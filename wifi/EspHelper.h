/*
 * EspHelper.h - Library for the ESP8266 Module
 * Created by Shane Jansen, September 3 2015
 */
#ifndef EspHelper_h
#define EspHelper_h

#include <SoftwareSerial.h>

class EspHelper {
  public:
    EspHelper(int, int, String, String, bool); //Constructor
    void makeConnection();
    void motionDetected();
    void sendAtCommand(String);
    void logOutput();
  private:
    int _RX, _TX;
    String _SSID, _PASSWORD;
    bool _DEBUG;
    SoftwareSerial *espSerial;
    String sendData(String, int);
    void serialPrint(String);
};

#endif
