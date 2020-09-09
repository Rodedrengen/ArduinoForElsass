/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

typedef uint8_t MediaKeyReport[2];

const MediaKeyReport KEY_MEDIA_PLAY_PAUSE1 = {8, 0};

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE1);

  }

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}