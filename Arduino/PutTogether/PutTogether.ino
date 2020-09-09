#include <WiFi.h>
#include <WebServer.h>

#include <BleKeyboard.h>

//input button ports
#define btn33 33
#define btn25 25
#define btn26 26
#define btn27 27

//Input button for config
#define onOff 32

//debounce time for buttons
long debouncing_time = 20;
volatile unsigned long last_micros;

//Symbols to be sent with keyboard presses
char symbol33 = 'a';
char symbol25 = 'b';
char symbol26 = ' ';
char symbol27 = 'q';

//Booleans to check what buttons have been pressed  
volatile boolean boolbtn33 = false;
volatile boolean boolbtn25 = false;
volatile boolean boolbtn26 = false;
volatile boolean boolbtn27 = false;

//Set the name of the bluetooth keyboard
BleKeyboard bleKeyboard("Elsass keyboard");

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

//Attach interupt methods
void IRAM_ATTR ISR_btn33(){boolbtn33 = true;}
void IRAM_ATTR ISR_btn25(){boolbtn25 = true;}
void IRAM_ATTR ISR_btn26(){boolbtn26 = true;}
void IRAM_ATTR ISR_btn27(){boolbtn27 = true;}

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

void setup() {
  Serial.begin(115200);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/dat1", handle_dat1);
  server.on("/dat2", handle_dat2);
  server.on("/dat3", handle_dat3);
  server.on("/dat4", handle_dat4);
  server.onNotFound(handle_NotFound);

  //Begin the BLE keyboard/start advertising the keyboard
  bleKeyboard.begin();
  delay(2000);
  
  server.begin();
  Serial.println("HTTP server started");
  //Attach interupt button pins
  pinMode(btn33, INPUT);
  pinMode(btn25, INPUT);
  pinMode(btn26, INPUT);
  pinMode(btn27, INPUT);
  //Attach attachInterupt
  attachInterrupt(btn33, ISR_btn33, RISING);
  attachInterrupt(btn25, ISR_btn25, RISING);
  attachInterrupt(btn26, ISR_btn26, RISING);
  attachInterrupt(btn27, ISR_btn27, RISING);
  //Config button
  pinMode(onOff, INPUT);
}
void loop() {
  if(digitalRead(onOff) == HIGH){
    server.handleClient();  
  }else{
    if(bleKeyboard.isConnected()){
      buttonsPresses();
    }
  }
  
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat1(){
  Serial.println("Handle_Dat1");
  if(server.hasArg("stringDat1")){
    symbol33 = server.arg("stringDat1").charAt(0);
    Serial.println(server.arg("stringDat1"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat2(){
  Serial.println("Handle_Dat2");
  if(server.hasArg("stringDat2")){
    symbol25 = server.arg("stringDat2").charAt(0);
    Serial.println(server.arg("stringDat2"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat3(){
  Serial.println("Handle_Dat3");
  if(server.hasArg("stringDat3")){
    symbol26 = server.arg("stringDat3").charAt(0);
    Serial.println(server.arg("stringDat3"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat4(){
  Serial.println("Handle_Dat4");
  if(server.hasArg("stringDat4")){
    symbol27 = server.arg("stringDat4").charAt(0);
    Serial.println(server.arg("stringDat4"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html><html><head> <meta name=viewport content=width=device-width,";
  ptr +="initial-scale=1.0, user-scalable=no> <title>LED Control</title> <style>html{font-family:";
  ptr +="Helvetica; display: inline-block; margin: 0px auto; text-align: center;}body{margin-top:" ;
  ptr +="50px;}h1{color: #444444; margin: 50px auto 30px;}h3{color: #444444; margin-bottom: 50px;}";
  ptr +="p{font-size: 14px; color: #888; margin-bottom: 10px;}</style></head><body> <h1>ESP32 Web Server</h1>";
  ptr +="<h3>Using Access Point(AP) Mode</h3> <form action=\"/dat1\"> <label for=\"stringDat1\">Dat1</label>" ;
  ptr +="<input type=\"text\" id=\"stringDat1\" name=\"stringDat1\" value=\"\"> <input type=\"submit\" value=\"Submit\">"; 
  ptr +="</form> <br><form action=\"/dat2\"> <label for=\"stringDat2\">Dat2</label> <input type=\"text\" id=\"stringDat2\""; 
  ptr +="name=\"stringDat2\" value=\"\"> <input type=\"submit\" value=\"Submit\"> </form> <br><form action=\"/dat3\">" ;
  ptr +="<label for=\"stringDat3\">Dat3</label> <input type=\"text\" id=\"stringDat3\" name=\"stringDat3\" value=\"\">" ;
  ptr +="<input type=\"submit\" value=\"Submit\"> </form> <br><form action=\"/dat4\"> <label for=\"stringDat4\">Dat4";
  ptr +="</label> <input type=\"text\" id=\"stringDat4\" name=\"stringDat4\" value=\"\"> <input type=\"submit\" value";
  ptr +="=\"Submit\"> </form></body></html>";
  return ptr;
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
