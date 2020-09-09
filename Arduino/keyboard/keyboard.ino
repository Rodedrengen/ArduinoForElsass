/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

const int touchPin = 4;
const int btnPin = 27;

const int threshold = 20;

int touchValue;
int btnValue;

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(btnPin, INPUT_PULLDOWN);
  
  bleKeyboard.begin();
  
}

void loop() {

  touchValue = touchRead(touchPin);
  btnValue = analogRead(btnPin);
  
  Serial.println(btnValue);
  
  if(bleKeyboard.isConnected()) {

    if(touchValue < threshold){
       bleKeyboard.print(" ");

    }
    if(btnValue == HIGH){
      bleKeyboard.print("a");
    }
  }
  
  delay(200);
  
}
