#include <Bounce2.h>

#define BUTTON_PIN 2

Bounce debouncer = Bounce();

int fallingEdge = 0;
int rissingEdge = 0;

void setup() {

  pinMode(BUTTON_PIN, INPUT);
  
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(25);
  
  Serial.begin(9600);
  Serial.println("Running....");
}

void loop() {
  debouncer.update();

  if(debouncer.fell() ){
    fallingEdge++;
    Serial.print("falling edge = ");
    Serial.println(fallingEdge);
  }
  if(debouncer.rose() ) {
    rissingEdge++;
    Serial.print("rissing edge = ");
    Serial.println(rissingEdge);
  }
}
