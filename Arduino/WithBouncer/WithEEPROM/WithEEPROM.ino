/*

Author: Simon Kjems Jørgensen
Organsation: Elsass Fonden 
Year: 08-08-2020

This is a BLEkeyboard that is configurable
to send most normal and fequently used keys.

It uses a webserver to be configured with
a simple webinterface. 

The esp32 must be configured to use huge 
partition (3mb no OTA / 1mb SPIFFS)
*/

//Includes 
//Webserver and wifi used to configurer the BleKeyboard
#include <WiFi.h>
#include <WebServer.h>

//Easy bounce handling
#include <Bounce2.h>

//Handle the bluetooth low energy keyboard
#include <BleKeyboard.h>

//read and write to flash memory
#include <EEPROM.h>

//Size of the EEPROM
#define EEPROM_SIZE 8

//Amount of buttons. Max of 8
#define numberOfButtons 4

//the amout of normal symbols to be sent
#define mapLength 96

//input button ports
#define btn33 33
#define btn25 25
#define btn26 26
#define btn27 27

//Input button for config
#define onOff 32

//Prototypes
//TODO remove these?
void checkBounce(Bounce debouncer, uint8_t symbol, uint8_t MediaValue1, uint8_t MediaValue2);
void handle_OnConnect();
void handling();
void handle_NotFound();
String sendHTML();
void mediaKeyOrSymbol();
void configurerEEPROM();

//Delay for the bouncers. 
const unsigned long bounceDelay = 25;

//Struct to translate from server response to hexcode
typedef struct{
  uint8_t hexCode;
  String serverTxt;
} translator;

//The array of predifened keys to be send with BleKeyboard.write
const uint8_t definedMediaKeys[16][2] = {
    {1, 0},   //MEDIA_NEXT_TRACK
    {2, 0},   //MEDIA_PREVIOUS_TRACK
    {4, 0},   //MEDIA_STOP
    {8, 0},   //MEDIA_PLAY_PAUSE
    {16, 0},  //MEDIA_MUTE
    {32, 0},  //MEDIA_VOLUME_UP
    {64, 0},  //MEDIA_VOLUME_DOWN
    {128, 0}, //MEDIA_WWW_HOME
    {0, 1},   //MEDIA_LOCAL_MACHINE_BROWSER
    {0, 2},   //MEDIA_CALCULATOR
    {0, 4},   //MEDIA_WWW_BOOKMARKS
    {0, 8},   //"MEDIA_WWW_SEARCH"
    {0, 16},  //MEDIA_WWW_STOP
    {0, 32},  //MEDIA_WWW_BACK
    {0, 64},  //"MEDIA_CONSUMER_CONTROL_CONFIGURATION"
    {0, 128}  //MEDIA_EMAIL_READER
};

//Desc of mediakeys
const String descriptionMediaKeys[16]={
  "MEDIA_NEXT_TRACK",
  "MEDIA_PREVIOUS_TRACK",
  "MEDIA_STOP",
  "MEDIA_PLAY_PAUSE",
  "MEDIA_MUTE",
  "MEDIA_VOLUME_UP",
  "MEDIA_VOLUME_DOWN",
  "MEDIA_WWW_HOME",
  "MEDIA_LOCAL_MACHINE_BROWSER",
  "MEDIA_CALCULATOR",
  "MEDIA_WWW_BOOKMARKS",
  "MEDIA_WWW_SEARCH",
  "MEDIA_WWW_STOP",
  "MEDIA_WWW_BACK",
  "MEDIA_CONSUMER_CONTROL_CONFIGURATION",
  "MEDIA_EMAIL_READER"
};
//Saved values pulled from EEPROM
String savedValues[8]{
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  ""
};

int savedValuesIndex = 0;

//The decimal code and it's symbol to be send with BleKeyboard.write
const translator serverToHex[mapLength] = {
    {97, "a"},
    {98, "b"},
    {99, "c"},
    {100, "d"},
    {101, "e"},
    {102, "f"},
    {103, "g"},
    {104, "h"},
    {105, "i"},
    {106, "j"},
    {107, "k"},
    {108, "l"},
    {109, "m"},
    {110, "n"},
    {111, "o"},
    {112, "p"},
    {113, "q"},
    {114, "r"},
    {115, "s"},
    {116, "t"},
    {117, "u"},
    {118, "v"},
    {119, "w"},
    {120, "x"},
    {121, "y"},
    {122, "z"},
    {49, "1"},
    {50, "2"},
    {51, "3"},
    {52, "5"},
    {53, "5"},
    {54, "6"},
    {55, "7"},
    {56, "8"},
    {57, "9"},
    {48, "0"},
    {33, "!"},
    {34, "\""},
    {35, "#"},
    {47, "/"},
    {40, "("},
    {41, ")"},
    {61, "="},
    {63, "?"},
    {96, "`"},
    {94, "^"},
    {42, "*"},
    {45, "-"},
    {95, "_"},
    {46, "."},
    {58, ":"},
    {59, ";"},
    {44, ","},
    {60, "<"},
    {62, ">"},
    {124, "|"},
    {123, "{"},
    {125, "}"},
    {91, "["},
    {93, "]"},
    {64, "@"},
    {0x80, "LEFT_CTRL"},
    {0x81, "LEFT_SHIFT"},
    {0x82, "LEFT_ALT"},
    {0x83, "LEFT_GUI"},
    {0x84, "RIGHT_CTRL"},
    {0x85, "RIGHT_SHIFT"},
    {0x86, "RIGHT_ALT"},
    {0x87, "RIGHT_GUI"},
    {0xDA, "UP_ARROW"},
    {0xD9, "DOWN_ARROW"},
    {0xD8, "LEFT_ARROW"},
    {0xD7, "RIGHT_ARROW"},
    {0xB2, "BACKSPACE"},
    {0xB3, "TAB"},
    {0xB0, "RETURN"},
    {0xB1, "ESC"},
    {0xD1, "INSERT"},
    {0xD4, "DELETE"},
    {0xD3, "PAGE_UP"},
    {0xD6, "PAGE_DOWN"},
    {0xD2, "HOME"},
    {0xD5, "END"},
    {0xC1, "CAPS_LOCK"},
    {0xC2, "F1"},
    {0xC3, "F2"},
    {0xC4, "F3"},
    {0xC5, "F4"},
    {0xC6, "F5"},
    {0xC7, "F6"},
    {0xC8, "F7"},
    {0xC9, "F8"},
    {0xCA, "F9"},
    {0xCB, "F10"},
    {0xCC, "F11"},
    {0xCD, "F12"}};

//Array used to store interger data from the server
uint8_t dats[8] = {
    0, 0, 0, 0, 0, 0, 0, 0};

//The names of the arguments used on the server. 
const String arguments[8] = {
    "stringDat0",
    "stringDat1",
    "stringDat2",
    "stringDat3",
    "stringDat4",
    "stringDat5",
    "stringDat6",
    "stringDat7"};

//default symbols to be sent with keyboard presses
uint8_t symbols[8] = {
    0, 0, 0, 0, 0, 0, 0, 0};

//Mediakeys for each button
uint8_t SendMediaKeys[8][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}};

//Set the name of the bluetooth keyboard
BleKeyboard bleKeyboard("Elsass keyboard");

//Debouncers for all buttons
Bounce debouncers[8] = {
    {btn33, bounceDelay},
    {btn25, bounceDelay},
    {btn26, bounceDelay},
    {btn27, bounceDelay},
    {btn27, bounceDelay},
    {btn27, bounceDelay},
    {btn27, bounceDelay},
    {btn27, bounceDelay}};

/* Put your SSID & Password */
const char *ssid = "ESP32";        // Enter SSID here
const char *password = "12345678"; //Enter Password here

//Webserver config
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//Create webserver on port 80
WebServer server(80);

//The string the response is stored in
String response = "";

bool configureServer = false;

void setup(){
  //Begin serial communication on speed 115200
  Serial.begin(115200);

  //Setup Wifi
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  //To limit errors
  delay(100);

  //Handle on connect to config
  server.on("/config", handle_OnConnect);
  //Handle form action
  server.on("/action", handling);
  //Server error
  server.onNotFound(handle_NotFound);

  //Begin the BLE keyboard/start advertising the keyboard
  bleKeyboard.begin();
  Serial.println("Blekeyboard begun");
  //To limit errors
  delay(2000);

  //Start the server
  server.begin();
  Serial.println("Server begun at:");
  Serial.print(local_ip);

  //Button pins
  pinMode(btn33, INPUT);
  pinMode(btn25, INPUT);
  pinMode(btn26, INPUT);
  pinMode(btn27, INPUT);

  //Config button
  pinMode(onOff, INPUT);

  //Create the HTML response
  response = sendHTML();
  //To limit errors
  delay(1000);

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  
  //Server or EEPROM data
  configureServer = true;

  configurerEEPROM();

  //Total delay 3100 ms
}
void loop(){
  //Update all decbouncers
  for (int k = 0; k < numberOfButtons; k++)  {
    debouncers[k].update();
  }

  //Check if we're using the server to configure
  if (configureServer){
    server.handleClient();
  }
    
  if (bleKeyboard.isConnected() && !configureServer)    {
    for (int i = 0; i < numberOfButtons; i++){
      checkBounce(debouncers[i], symbols[i], SendMediaKeys[i][0], SendMediaKeys[i][1]);
    }
  }
}
void configurerEEPROM(){
  Serial.println("Configurer EEPROM");
  for(int y = 0; y < numberOfButtons; y++){
      dats[y] = EEPROM.read(y);
      Serial.println(dats[y]);
    }
    mediaKeyOrSymbol();
}

String sendHTML(){
  Serial.println("Send HTML");
  String ptr = R"HTML(<!DOCTYPE html><html><head><meta name="viewport" http-equiv="Content-Type"  content="width=device-width, initial-scale=1.0, charset=utf-8"> 
<title>Elsass</title><link rel="icon" href="data:;base64,="><style>input[type=submit]{width: 100%;background-color:#4CAF50;color:white;
padding:14px 20px;margin: 8px 0;border: none;border-radius: 4px;cursor: pointer;}select{margin-top: 4px;margin-left:100px;width: 50%;
}.select-selected {background-color: DodgerBlue;}div {max-width: 400px;width: 100%;margin: 0 auto;position: relative;} </style></head>
<body><div id = "content"></div></body>
<script>var selected = ["LEFT_CTRL","#","1","MUTE"]
var strings = ["a","b","c","d","e ","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","1",
"2","3","4","5","6","7","8","9","0","!","\"","#","/","(",")","=","?","`","^","*","-","_",".",":",";",",","<",">","|","{","}",
"[","]","@","LEFT_CTRL","LEFT_SHIFT","LEFT_ALT","LEFT_GUI","RIGHT_CTRL","RIGHT_SHIFT","RIGHT_ALT",
"RIGHT_GUI","UP_ARROW","DOWN_ARROW","LEFT_ARROW","RIGHT_ARROW","BACKSPACE","TAB","RETURN","ESC",
"INSERT","DELETE","PAGE_UP","PAGE_DOWN","HOME","END","CAPS_LOCK","F1","F2","F3","F4",
"F5","F6","F7","F8","F9","F10","F11","F12","NEXT_TRACK","PREVIOUS_TRACK","STOP",
"PLAY_PAUSE","MUTE","VOLUME_UP","VOLUME_DOWN","WWW_HOME","LOCAL_MACHINE_BROWSER","CALCULATOR","WWW_BOOKMARKS","WWW_SEARCH",
"WWW_STOP","WWW_BACK","CONSUMER_CONTROL_CONFIGURATION","EMAIL_READER"];

var k;
var i;
var text="";

  for (k = 0; k < 4; k++){
    
        text += "<form action=\"/action\" onsubmit=\"alert('Konfiguration komplet');\"><select name=\"stringDat";
        text += k;
        text += "\">";
    
        for (i = 0; i < strings.length; i++) {
            text += "<option value=\"";
            text += i;
            if(selected[k] == strings[i]){
                text += "\" selected=>";
            }else{
                text += "\">";
            }
            text += strings[i];
            text += "</option>";
        } 
        text += "</select>"; 
    }

    text += "<input type=\"submit\" value=\"Submit\"></form>";
    document.getElementById("content").innerHTML = text;

    </script>

</html>)HTML";
  return ptr;
}

void handle_OnConnect(){
  //Handling on connection
  Serial.println("On connect");
  server.send(200, "text/html", response);
}
void handling(){
  //Handles the form response
  Serial.println("Handling....");

  //converts http value to integer
  for (int i = 0; i < numberOfButtons; i++){
    dats[i] = server.arg(arguments[i]).toInt();
    EEPROM.write(i, dats[i]);
    Serial.println(dats[i]);
  }
  EEPROM.commit;

  mediaKeyOrSymbol();

  configureServer = false;

  //Sends an "empty" response back
  server.send(200, "text/html", "Thank you");
}
void handle_NotFound(){
  //Handles not found errors. 
  Serial.println("Handle not found");
  server.send(404, "text/plain", "Did not find anything");
}

void mediaKeyOrSymbol(){
  //Checks what symbol / mediakey is to be used
  Serial.println("mediaKeyOrSymbol...");
  for (int k = 0; k < numberOfButtons; k++)  {

    //if dats is greater than mapLength. Mediakeys are to be used instead.
    if (dats[k] >= mapLength){
      SendMediaKeys[k][0] = definedMediaKeys[dats[k] - mapLength][0];
      SendMediaKeys[k][1] = definedMediaKeys[dats[k] - mapLength][1];
      savedValues[savedValuesIndex] = descriptionMediaKeys[dats[k] - mapLength]; 
      savedValuesIndex++;
    }else{
      symbols[k] = serverToHex[dats[k]].hexCode;
      savedValues[k] = serverToHex[dats[k]].serverTxt;
    }
  }
}

void checkBounce(Bounce debouncer, uint8_t symbol, uint8_t MediaValue1, uint8_t MediaValue2){

  uint8_t tempArr[2] = {MediaValue1, MediaValue2};
  Serial.println("checkBounce");
  Serial.print(tempArr[0]);
  Serial.print(" ");
  Serial.println(tempArr[1]);

  if (debouncer.rose()){
    if (symbol == 0){
      bleKeyboard.press(tempArr);
    }else{
      bleKeyboard.press(symbol);
    }
  }
  if (debouncer.fell()){
    if (symbol == 0){
      bleKeyboard.release(tempArr);
    }else{
      bleKeyboard.release(symbol);
    }
  }
}
