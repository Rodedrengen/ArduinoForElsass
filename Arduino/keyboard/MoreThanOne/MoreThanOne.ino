/*
@Title BLE keyboard
@Author Simon Kjems Jørgensen
@Institute Elsass Fonden
@Date 04-06-2020
*/ 

#include <BleKeyboard.h>

#define btn33 33
#define btn25 25
#define btn26 26
#define btn27 27

//Se the name of the bluetooth keyboard
BleKeyboard bleKeyboard("Elsass keyboard");

String symbol33 = "a";
String symbol25 = "b";
String symbol26 = " ";
String symbol27 = "q";

//Set the old button state to be LOW/false; which means not pressed
boolean oldPinState33 = LOW;
boolean oldPinState25 = LOW;
boolean oldPinState26 = LOW;
boolean oldPinState27 = LOW; 

void setup() {
  //Start the Serial communication 
  Serial.begin(115200);
  //Send this message to the computer
  Serial.println("Starting BLE work!");
  //Begin the BLE keyboard/start advertising the keyboard
  bleKeyboard.begin();
  delay(2000);
  //Make the buttons pins INPUT. They are hardware pulldown.
  pinMode(btn33, INPUT);
  pinMode(btn25, INPUT);
  pinMode(btn26, INPUT);
  pinMode(btn27, INPUT);
}

void loop() {
  //if the keyboard is connected to a device
  if (bleKeyboard.isConnected()) {
    //Button 33
    if (digitalRead(btn33) == HIGH) {
      if(oldPinState33 == LOW){
        
        bleKeyboard.print(symbol33);
      }
      oldPinState33 = HIGH;
    }else{
      oldPinState33 = LOW;
    }
    //Button 25
    if (digitalRead(btn25) == HIGH) {
      if(oldPinState25 == LOW){
        
        bleKeyboard.print(symbol25);
      }
      oldPinState25 = HIGH;
    }else{
      oldPinState25 = LOW;
    }
    //Button 26
    if (digitalRead(btn26) == HIGH) {
      if(oldPinState26 == LOW){
        
        bleKeyboard.print(symbol26);
      }
      oldPinState26 = HIGH;
    }else{
      oldPinState26 = LOW;
    }
    //Button 27
    if (digitalRead(btn27) == HIGH) {
      if(oldPinState27 == LOW){
        
        bleKeyboard.print(symbol27);
      }
      oldPinState27 = HIGH;
    }else{
      oldPinState27 = LOW;
    }
  }else{
    Serial.println("Not connected");
  }
}
