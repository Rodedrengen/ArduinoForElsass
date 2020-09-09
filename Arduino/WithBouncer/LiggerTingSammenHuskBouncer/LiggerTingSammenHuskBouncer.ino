#include <Bounce2.h>

#include <BleKeyboard.h>

//input button ports
#define btn33 33
#define btn25 25
#define btn26 26
#define btn27 27

unsigned long bounceDelay = 25;

//Symbols to be sent with keyboard presses
uint8_t symbol33= 0xDA; //Shift
uint8_t symbol25= 0xD9; //Ctrl
uint8_t symbol26= 0xD8; //a
uint8_t symbol27= 0xD7; //b

//Set the name of the bluetooth keyboard
BleKeyboard bleKeyboard("Elsass keyboard");

Bounce debouncer33 = Bounce(btn33,bounceDelay);
Bounce debouncer25 = Bounce(btn25,bounceDelay);
Bounce debouncer26 = Bounce(btn26,bounceDelay);
Bounce debouncer27 = Bounce(btn27,bounceDelay);

void setup() {
  Serial.begin(115200);

  //Begin the BLE keyboard/start advertising the keyboard
  bleKeyboard.begin();
  delay(2000);
 
  //Attach interupt button pins
  pinMode(btn33, INPUT);
  pinMode(btn25, INPUT);
  pinMode(btn26, INPUT);
  pinMode(btn27, INPUT);
  
}
void loop() {
  debouncer33.update();
  debouncer25.update();
  debouncer26.update();
  debouncer27.update();
  
  if(bleKeyboard.isConnected()) {
    checkBounce(debouncer33, symbol33);
    checkBounce(debouncer25, symbol25);
    checkBounce(debouncer26, symbol26);
    checkBounce(debouncer27, symbol27);
  }
}

void checkBounce(Bounce debouncer, uint8_t symbol){

  if(debouncer.rose()){
    Serial.print("key press");
    Serial.println(symbol);
    bleKeyboard.press(symbol);
  }
  if(debouncer.fell()){
    Serial.print("key release");
    Serial.println(symbol);
    bleKeyboard.release(symbol);
  }
  
}
