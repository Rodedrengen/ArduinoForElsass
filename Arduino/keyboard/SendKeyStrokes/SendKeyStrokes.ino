/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

//Input button for config
#define onOff 32

BleKeyboard bleKeyboard;

const uint8_t key = 0xD8;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  delay(2000);
  
  //Config button
  pinMode(onOff, INPUT);
}

void loop() {
  if(bleKeyboard.isConnected()) {
    if(digitalRead(onOff == HIGH)){
        
        Serial.println("Sending Enter key...");
        bleKeyboard.print("ø");

        delay(2000);
    }
  }    
}
