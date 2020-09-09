#include <WiFi.h>
#include <WebServer.h>

#define onOff 32

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

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
  
  server.begin();
  Serial.println("HTTP server started");
  pinMode(onOff, INPUT);
}
void loop() {
  if(digitalRead(onOff) == HIGH){
    server.handleClient();  
  }
  
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat1(){
  Serial.println("Handle_Dat1");
  if(server.hasArg("stringDat1")){
    Serial.println(server.arg("stringDat1"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat2(){
  Serial.println("Handle_Dat2");
  if(server.hasArg("stringDat2")){
    Serial.println(server.arg("stringDat2"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat3(){
  Serial.println("Handle_Dat3");
  if(server.hasArg("stringDat3")){
    Serial.println(server.arg("stringDat3"));
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_dat4(){
  Serial.println("Handle_Dat4");
  if(server.hasArg("stringDat4")){
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
