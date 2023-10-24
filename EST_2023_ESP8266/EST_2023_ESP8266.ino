
//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

//Include Lib for Arduino to Nodemcu
// #include <SoftwareSerial.h>
#include <ArduinoJson.h>
float t, h, c;
//D6 = Rx & D5 = Tx
// SoftwareSerial nodemcu(D6, D5);


void arduinoSetup() {
  // Initialize Serial port
  Serial.begin(9600);
  // nodemcu.begin(9600);
  // while (!Serial) continue;
}

void arduinoLoop() {
  DynamicJsonDocument data(1000);
  DeserializationError error = deserializeJson(data, Serial);
  if (error)
    return;
  //  value = data["value"];



  h = data["humidity"];
  t = data["temperature"];
  c = data["concentration"];
}
