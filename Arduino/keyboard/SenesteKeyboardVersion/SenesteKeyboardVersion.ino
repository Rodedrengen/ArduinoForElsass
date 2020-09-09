/*
@Title BLE keyboard
@Author Simon Kjems Jørgensen
@Institute Elsass Fonden
@Date 04-06-2020
Hardware: ESP32 
Board: ESP32 dev module
*/

#include <BleKeyboard.h>

#define btn33 33
#define btn25 25
#define btn26 26
#define btn27 27

//Se the name of the bluetooth keyboard
BleKeyboard bleKeyboard("Elsass keyboard");

long debouncing_time = 20;
volatile unsigned long last_micros;

char symbol33 = 'a';
char symbol25 = 'b';
char symbol26 = ' ';
char symbol27 = 'q';

volatile boolean boolbtn33 = false;
volatile boolean boolbtn25 = false;
volatile boolean boolbtn26 = false;
volatile boolean boolbtn27 = false;

void IRAM_ATTR ISR_btn33(){
    boolbtn33 = true; 
}
void IRAM_ATTR ISR_btn25(){
 
    boolbtn25 = true; 
   
}
void IRAM_ATTR ISR_btn26(){

    boolbtn26 = true; 
 
}
void IRAM_ATTR ISR_btn27(){

    boolbtn27 = true; 

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

  attachInterrupt(btn33, ISR_btn33, RISING);
  attachInterrupt(btn25, ISR_btn25, RISING);
  attachInterrupt(btn26, ISR_btn26, RISING);
  attachInterrupt(btn27, ISR_btn27, RISING);
}

void loop()
{
  //if the keyboard is connected to a device
  if (bleKeyboard.isConnected()){
   buttonsPresses();
  }
}

void buttonsPresses(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if(boolbtn33){
       bleKeyboard.write(symbol33);
       boolbtn33 = false;
     }
      if(boolbtn25){
       bleKeyboard.write(symbol25);
       boolbtn25 = false;
     }
      if(boolbtn26){
       bleKeyboard.write(symbol26);
       boolbtn26 = false;
     }
      if(boolbtn27){
       bleKeyboard.write(symbol27);
       boolbtn27 = false;
     }
     last_micros = micros();  
  }
    
}
