
//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

//Include Lib for Arduino to Nodemcu
// #include <SoftwareSerial.h>
#include <ArduinoJson.h>
float t, h, c;
//D6 = Rx & D5 = Tx
// SoftwareSerial nodemcu(D6, D5);


#define Max_Temp 40
#define Max_Hum 70
#define Max_Dust 3000
#define Beeb_Duration 1000

void arduinoSetup() {
  // Initialize Serial port
  Serial.begin(9600);
  // nodemcu.begin(9600);
  // while (!Serial) continue;
}

void arduinoLoop() {

  rxArduino();
  txArduino();
}
void rxArduino() {

  DynamicJsonDocument data(1000);
  DeserializationError error = deserializeJson(data, Serial);
  if (error)
    return;

  h = data["humidity"];
  t = data["temperature"];
  c = data["concentration"];
}
void txArduino() {
  if (t >= Max_Temp || h >= Max_Hum || c >= Max_Dust) {
    DynamicJsonDocument control(64);
    control["buzzer"] = 1;
    control["duration"] = Beeb_Duration;
    serializeJson(control, Serial);
  }
}