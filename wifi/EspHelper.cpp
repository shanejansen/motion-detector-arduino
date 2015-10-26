/*
 * EspHelper.cpp - Library for the ESP8266 Module
 * Created by Shane Jansen, September 3 2015
 */
#include "Arduino.h"
#include "EspHelper.h"

EspHelper::EspHelper (int RX, int TX, String SSID, String PASSWORD, bool DEBUG) {
  espSerial = new SoftwareSerial(RX, TX);
  espSerial->begin(9600);
  _RX = RX;
  _TX = TX;
  _SSID = SSID;
  _PASSWORD = PASSWORD;
  _DEBUG = DEBUG;
}

/**
 * BLOCKS THE MAIN THREAD
 * Checks the ESP's WiFi connection and
 * makes one if necessary.
 */
void EspHelper::makeConnection() {
  String ipAddress = sendData("AT+CIFSR\r\n", 5000);
  if (ipAddress.indexOf("0.0.0.0") == -1) {
    serialPrint("ESP is already connected.");
    sendData("AT+CIFSR\r\n", 2000);
  }
  else {
    serialPrint("ESP is not connected; making connection.");
    sendData("AT+RST\r\n", 5000);
    sendData("AT+CWMODE=1\r\n", 2000);
    String creds = "AT+CWJAP=\"" +_SSID + "\",\"" + _PASSWORD + "\"\r\n";
    sendData(creds, 10000);
    sendData("AT+CIFSR\r\n", 2000);
  }
}

/**
 * BLOCKS THE MAIN THREAD
 * Sends data to the ESP and captures
 * the response in a single string. The
 * timeout is how long to wait for the response.
 */
String EspHelper::sendData(String command, int timeout) {
  String response = "";
  espSerial->print(command);
  long int time = millis();
  while((time + timeout) > millis()) { //if (ipAddress.indexOf("0.0.0.0") == -1)
    while(espSerial->available()) {
      char c = espSerial->read();
      response += c;
    }  
  }
  serialPrint(response);
  return response;
}

/**
 * BLOCKS THE MAIN THREAD
 * Makes a GET request which, in turn,
 * sends a push notification using the
 * Parse API.
 */
void EspHelper::motionDetected() {
  sendData("AT+CIPSTART=\"TCP\",\"sjjapps.com\",80\r\n", 2000);
  String request = "GET http://www.sjjapps.com/house-control/path-checker\r\n";
  String cipSend = "AT+CIPSEND=";
  cipSend += request.length() - 1;
  sendData(cipSend += "\r\n", 2000);
  sendData(request, 5000);
}

/**
 * Sends a command 
 */
void EspHelper::sendAtCommand(String command) {
  espSerial->println(command);
}

/**
 * Logs all of the ESP's output
 * to the Serial. Should be called
 * from "loop".
 */
void EspHelper::logOutput() {
  if(_DEBUG && espSerial->available()) {
    while(espSerial->available()) {
      char c = espSerial->read();
      Serial.write(c);
    }  
  }
}

/**
 * Print to the Serial if we
 * are in DEBUG mode.
 */
void EspHelper::serialPrint(String data) {
  if (_DEBUG) Serial.println(data);
}

