#include <WiFi.h>
#include <WebServer.h>

#include <stdlib.h>

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

//Using it to convert from input to HEX
char temp[25] = "1234";

//Int to print
unsigned long int s33 = 0x20;
unsigned long int s25 = 0x20;
unsigned long int s26 = 0x20;
unsigned long int s27 = 0x20;

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
  server.on("/dat1", handling);
  server.on("/dat2", handling);
  server.on("/dat3", handling);
  server.on("/dat4", handling);
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

unsigned long int convertString(String argument){
  unsigned long int returnparam;

  if(server.hasArg(argument)) {
    server.arg(argument).toCharArray(temp,25);
    returnparam = strtoul(temp,NULL,16);
  }
  
  return returnparam; 
}

void handling(){
  Serial.println("Handling");
  
  if(server.hasArg("stringDat1")){
    Serial.println(server.arg("stringDat1"));
    s33 = convertString("stringDat1");
    
  }else if(server.hasArg("stringDat2")){
    Serial.println(server.arg("stringDat2"));
    s25 = convertString("stringDat2");
    
  }else if(server.hasArg("stringDat3")){
    Serial.println(server.arg("stringDat3"));
    s26 = convertString("stringDat3");
    
  }else if(server.hasArg("stringDat4")){
    Serial.println(server.arg("stringDat4"));
    s27 = convertString("stringDat4");
    
  }else{
     server.send(404, "text/plain", "Not found");
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(){
   String ptr = R"HTML(<!DOCTYPE html><html><head><meta name=viewport content=width=device-width, initial-scale=1.0, user-scalable=no>
    <title>LED Control</title><style>html{font-family: Helvetica;display: inline-block;margin: 0px auto;
    text-align: center;}body{margin-top: 50px;}</style></head>

<body>

    <form action="/dat1"> 
        <label for="stringDat1">Dat1</label>
        <select id="stringDat1" name="stringDat1">
            <option value="0xD8">Venstre pil</option>
            <option value="0xB0">Enter</option>
            <option value="0x04">a</option>
            <option value="0x2c">mellemrum</option>
        </select>
        <input type="submit">
    </form>

    <form action="/dat2"> 
        <label for="stringDat2">Dat2</label>
        <select id="stringDat2" name="stringDat2">
            <option value="0xD8">Venstre pil</option>
            <option value="0xB0">Enter</option>
            <option value="0x04">a</option>
            <option value="0x2c">mellemrum</option>
        </select>
        <input type="submit">
    </form>

    <form action="/dat3"> 
        <label for="stringDat3">Dat1</label>
        <select id="stringDat3" name="stringDat3">
            <option value="0xD8">Venstre pil</option>
            <option value="0xB0">Enter</option>
            <option value="0x04">a</option>
            <option value="0x2c">mellemrum</option>
        </select>
        <input type="submit">
    </form>

    <form action="/dat4"> 
        <label for="stringDat4">Dat1</label>
        <select id="stringDat4" name="stringDat4">
            <option value="0xD8">Venstre pil</option>
            <option value="0xB0">Enter</option>
            <option value="0x04">a</option>
            <option value="0x2c">mellemrum</option>
        </select>
        <input type="submit">
    </form>
</body>

</html>)HTML";
return ptr;  
}
/*String SendHTML(){
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
*/
void buttonsPresses(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if(boolbtn33){
       Serial.println(s33);
       bleKeyboard.write(s33);
       boolbtn33 = false;
     }
      if(boolbtn25){
       Serial.println(s25);
       bleKeyboard.write('a');
       boolbtn25 = false;
     }
      if(boolbtn26){
        Serial.println(s26);
        Serial.println(0x04);
       bleKeyboard.write(s26);
       boolbtn26 = false;
     }
      if(boolbtn27){
        Serial.println(s27);
       bleKeyboard.write(s27);
       boolbtn27 = false;
     }

     last_micros = micros();

     
  }
}
