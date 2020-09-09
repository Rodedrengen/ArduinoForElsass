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

char symbol33 = 'a';
char symbol25 = 'b';
char symbol26 = ' ';
char symbol27 = 'q';

//Set the old button state to be LOW/false; which means not pressed
boolean oldPinState33 = LOW;
boolean oldPinState25 = LOW;
boolean oldPinState26 = LOW;
boolean oldPinState27 = LOW;

void IRAM_ATTR ISR_btn33(){
  Serial.println("33");
  if (oldPinState33 == LOW){
    bleKeyboard.write(symbol33);
    oldPinState33 == HIGH;
    
  }else{
    oldPinState33 == HIGH;
  }
}
void IRAM_ATTR ISR_btn25(){
  Serial.println("25");
  if (oldPinState25 == LOW){
    bleKeyboard.write(symbol25);
    oldPinState25 == HIGH;
    
  }else{
    oldPinState25 == HIGH;
  }
}
void IRAM_ATTR ISR_btn26(){
  Serial.println("26");
  if (oldPinState26 == LOW){
    bleKeyboard.write(symbol26);
    oldPinState26 == HIGH;
    
  }else{
    oldPinState26 == HIGH;
  }
}
void IRAM_ATTR ISR_btn27(){
  Serial.println("27");
  if (oldPinState27 == LOW){
    bleKeyboard.write(symbol27);
    oldPinState27 == HIGH;
   
  }else{
    oldPinState27 == HIGH;
  }
}

void setup()
{
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

  attachInterrupt(btn33, ISR_btn33, HIGH);
  attachInterrupt(btn25, ISR_btn25, HIGH);
  attachInterrupt(btn26, ISR_btn26, HIGH);
  attachInterrupt(btn27, ISR_btn27, HIGH);
}

void loop()
{
  //if the keyboard is connected to a device
  if (bleKeyboard.isConnected()){
   
  }
  else
  {
    Serial.println("Not connected");
  }
}
