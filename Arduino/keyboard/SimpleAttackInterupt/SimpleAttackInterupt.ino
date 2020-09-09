//Made for the ATMega 32 might not work with ESP32

#define btn1 2
boolean state = false;

void ISR_btn1(){
  state = !state;
  Serial.println("A");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // put your main code here, to run repeatedly:
  pinMode(btn1, INPUT);

  attachInterrupt(digitalPinToInterrupt(btn1), ISR_btn1, RISING);

  Serial.println("Starting up");
}

void loop() {
 Serial.println(state);
 delay(500);
}
