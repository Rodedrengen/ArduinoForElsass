
void setup() {
  pinMode(27, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("----");
  
  digitalWrite(27, HIGH);
  delay(2000);
  digitalWrite(27, LOW);
  delay(2000);
}
