// const int buzzerPin = 6;

unsigned long alarmStartTime = 0;
unsigned long alarmDuration = 0;
bool isAlarmActive = false;
void alarmSetup() {
  pinMode(buzzerPin, OUTPUT);
}
void alarmLoop() {

  StaticJsonDocument<64> control;
  DeserializationError error = deserializeJson(control, Serial);
  if (error) {
    nodemcu.print("Error: ");
    Serial.println(error.c_str());
    return;
  }

  int buzzerState = control["buzzer"];
  alarmDuration = control["duration"];

  if (buzzerState == 1 && !isAlarmActive) {
    alarmStartTime = millis();
    isAlarmActive = true;
    playTune();
  }

  if (isAlarmActive && millis() - alarmStartTime >= alarmDuration) {
    stopTune();
    isAlarmActive = false;
  }
}

void playTune() {
  tone(buzzerPin, 440);  // Play A4 note
}

void stopTune() {
  noTone(buzzerPin);
}