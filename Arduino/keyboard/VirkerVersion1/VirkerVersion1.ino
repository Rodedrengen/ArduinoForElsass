/*
@Title BLE keyboard, arcade button to phone
@Author Geert Roumen
@Institute Umeå Institute of Design
@Date 29-01-2020
 
@Hardware
 
Any ESP32 dev board would do, but I used the:
Geekworm EASY KIT ESP32-C1
https://geekworm.com/products/official-geekworm-esp32-development-board

Arcade button
Adafruit Large Arcade button white
https://www.adafruit.com/product/1192

D13       Button pin
3.3V      Connected to the other leg of the button
 
In this example sketch processing can send an RGB value to Arduino and Arduino will change the LED color of the NUMPIXELS connected neopixels
*/
#include <BleKeyboard.h>
//Se the name of the bluetooth keyboard (that shows up in the bluetooth menu of your device)
BleKeyboard bleKeyboard("Arcade Phone");

const int buttonPin = 27;
//Set the old button state to be LOW/false; which means not pressed
boolean oldPinState = LOW;

void setup() {
  //Start the Serial communication (with the computer at 115200 bits per second)
  Serial.begin(115200);
  //Send this message to the computer
  Serial.println("Starting BLE work!");
  //Begin the BLE keyboard/start advertising the keyboard (so phones can find it)
  bleKeyboard.begin();
  delay(2000);
  //Make the button pin an INPUT_PULLDOWN pin, which means that it is normally LOW, untill it is pressed/ connected to the 3.3V
  pinMode(buttonPin, INPUT);
}

void loop() {
  
  if (bleKeyboard.isConnected()) {
    
    Serial.println(digitalRead(buttonPin));
    
    //if the keyboard is connected to a device
    if (digitalRead(buttonPin) == HIGH) {
      
      if(oldPinState == LOW){
        
        Serial.println("BTN pin high");
        bleKeyboard.print(" ");
      }
      
      oldPinState = HIGH;
    }else{
      Serial.println("BTN pin LOW!");
      oldPinState = LOW;
    }
  }else{
    Serial.println("Not connected");
  }
}
