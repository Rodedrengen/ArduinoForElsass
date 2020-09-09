#include <WiFi.h>
#include <WebServer.h>

#include <Bounce2.h>

#include <BleKeyboard.h>

#define numberOfButtons 4

#define mapLength 96

//input button ports
#define btn33 33
#define btn25 25
#define btn26 26
#define btn27 27

//Input button for config
#define onOff 32

//Prototypes
void checkBounce(Bounce debouncer, uint8_t symbol, uint8_t MediaValue1, uint8_t MediaValue2);
void handle_OnConnect();
void handling();
void handle_NotFound();
String sendHTML();

//Bounce delay
unsigned long bounceDelay = 25;

typedef struct
{
  uint8_t hexCode;
  String serverTxt;
} translator;

uint8_t definedMediaKeys[16][2] = {
    {1, 0},   //KEY_MEDIA_NEXT_TRACK
    {2, 0},   //KEY_MEDIA_PREVIOUS_TRACK
    {4, 0},   //KEY_MEDIA_STOP
    {8, 0},   //KEY_MEDIA_PLAY_PAUSE
    {16, 0},  //KEY_MEDIA_MUTE
    {32, 0},  //KEY_MEDIA_VOLUME_UP
    {64, 0},  //KEY_MEDIA_VOLUME_DOWN
    {128, 0}, //KEY_MEDIA_WWW_HOME
    {0, 1},   //KEY_MEDIA_LOCAL_MACHINE_BROWSER
    {0, 2},   //KEY_MEDIA_CALCULATOR
    {0, 4},   //KEY_MEDIA_WWW_BOOKMARKS
    {0, 8},   //KEY_MEDIA_WWW_SEARCH
    {0, 16},  //KEY_MEDIA_WWW_STOP
    {0, 32},  //KEY_MEDIA_WWW_BACK
    {0, 64},  //KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION
    {0, 128}  //KEY_MEDIA_EMAIL_READER
};

translator serverToHex[mapLength] = {
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
    {34, "kjk\"jk"},
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
    {0x80, "KEY_LEFT_CTRL"},
    {0x81, "KEY_LEFT_SHIFT"},
    {0x82, "KEY_LEFT_ALT"},
    {0x83, "KEY_LEFT_GUI"},
    {0x84, "KEY_RIGHT_CTRL"},
    {0x85, "KEY_RIGHT_SHIFT"},
    {0x86, "KEY_RIGHT_ALT"},
    {0x87, "KEY_RIGHT_GUI"},
    {0xDA, "KEY_UP_ARROW"},
    {0xD9, "KEY_DOWN_ARROW"},
    {0xD8, "KEY_LEFT_ARROW"},
    {0xD7, "KEY_RIGHT_ARROW"},
    {0xB2, "KEY_BACKSPACE"},
    {0xB3, "KEY_TAB"},
    {0xB0, "KEY_RETURN"},
    {0xB1, "KEY_ESC"},
    {0xD1, "KEY_INSERT"},
    {0xD4, "KEY_DELETE"},
    {0xD3, "KEY_PAGE_UP"},
    {0xD6, "KEY_PAGE_DOWN"},
    {0xD2, "KEY_HOME"},
    {0xD5, "KEY_END"},
    {0xC1, "KEY_CAPS_LOCK"},
    {0xC2, "KEY_F1"},
    {0xC3, "KEY_F2"},
    {0xC4, "KEY_F3"},
    {0xC5, "KEY_F4"},
    {0xC6, "KEY_F5"},
    {0xC7, "KEY_F6"},
    {0xC8, "KEY_F7"},
    {0xC9, "KEY_F8"},
    {0xCA, "KEY_F9"},
    {0xCB, "KEY_F10"},
    {0xCC, "KEY_F11"},
    {0xCD, "KEY_F12"}};

//integer data from the server
uint8_t dats[8] = {
    0, 0, 0, 0, 0, 0, 0, 0};

//The arguments for the server
String arguments[8] = {
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

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

String response = "";

void setup()
{
  Serial.begin(115200);

  //Setup Wifi
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
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
  delay(1000);
}
void loop(){
  //Update all decbouncers
  for (int k = 0; k < numberOfButtons; k++)  {
    debouncers[k].update();
  }

  //Check if the server is to be configured
  if (digitalRead(onOff) == HIGH)  {
    server.handleClient();
  }else{
    if (bleKeyboard.isConnected())    {
      for (int i = 0; i < numberOfButtons; i++){
        checkBounce(debouncers[i], symbols[i], SendMediaKeys[i][0], SendMediaKeys[i][1]);
      }
    }
  }
}

String sendHTML()
{
  String ptr = R"HTML(<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" http-equiv="Content-Type"  content="width=device-width, initial-scale=1.0, charset=utf-8"> 
        <title>Elsass</title>
        <link rel="icon" href="data:;base64,=">
        <style>
        
        input[type=submit] {
            width: 100%;
            background-color: #4CAF50;
            color: white;
            padding: 14px 20px;
            margin: 8px 0;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        select{
            margin-top: 4px;
            margin-left:100px;
            width: 50%;
        }
        .select-selected {
            background-color: DodgerBlue;
        }

        div {
            max-width: 400px;
            width: 100%;
            margin: 0 auto;
            position: relative;
          } 
        </style>

        
    </head>
    <body>
            <div id = "content">

            </div>

    </body>
    <script>
var strings = ["a",
    "b",
    "c",
    "d",
    "e ",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z",
    "1",
    "2",
    "3",
    "5",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "!",
    "\"",
    "#",
    "/",
    "(",
    ")",
    "=",
    "?",
    "`",
    "^",
    "*",
    "-",
    "_",
    ".",
    ":",
    ";",
    ",",
    "<",
    ">",
    "|",
    "{",
    "}",
    "[",
    "]",
    "@",
    "KEY_LEFT_CTRL",
    "KEY_LEFT_SHIFT",
    "KEY_LEFT_ALT",
    "KEY_LEFT_GUI",
    "KEY_RIGHT_CTRL",
    "KEY_RIGHT_SHIFT",
    "KEY_RIGHT_ALT",
    "KEY_RIGHT_GUI",
    "KEY_UP_ARROW",
    "KEY_DOWN_ARROW",
    "KEY_LEFT_ARROW",
    "KEY_RIGHT_ARROW",
    "KEY_BACKSPACE",
    "KEY_TAB",
    "KEY_RETURN",
    "KEY_ESC",
    "KEY_INSERT",
    "KEY_DELETE",
    "KEY_PAGE_UP",
    "KEY_PAGE_DOWN",
    "KEY_HOME",
    "KEY_END",
    "KEY_CAPS_LOCK",
    "KEY_F1",
    "KEY_F2",
    "KEY_F3",
    "KEY_F4",
    "KEY_F5",
    "KEY_F6",
    "KEY_F7",
    "KEY_F8",
    "KEY_F9",
    "KEY_F10",
    "KEY_F11",
    "KEY_F12",
    "NEXT_TRACK",
    "PREVIOUS_TRACK",
    "STOP",
    "PLAY_PAUSE",
    "MUTE",
    "VOLUME_UP",
    "VOLUME_DOWN",
    "WWW_HOME",
    "LOCAL_MACHINE_BROWSER",
    "CALCULATOR",
    "WWW_BOOKMARKS",
    "WWW_SEARCH",
    "WWW_STOP",
    "WWW_BACK",
    "CONSUMER_CONTROL_CONFIGURATION",
    "EMAIL_READER"
    ];

    var k;
    var i;
    var text = "";
    
    for (k = 0; k < 4; k++){
    
        text += "<form action=\"/action\" onsubmit=\"alert('Konfiguration komplet');\"><select name=\"stringDat";
        text += k;
        text += "\">";
    
        for (i = 0; i < strings.length; i++) {
            text += "<option value=\"";
            text += i;
            text += "\">";
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
  server.send(200, "text/html", responsem, );
}
void handling(){
  //Handles the form response
  Serial.println("Handling....");

  //converts http value to integer
  for (int i = 0; i < numberOfButtons; i++){
    dats[i] = server.arg(arguments[i]).toInt();
    Serial.println(dats[i]);
  }

  //Checks what symbol / mediakey is to be used
  for (int k = 0; k < numberOfButtons; k++)  {

    //if dats is greater than mapLength. Mediakeys are to be used instead.
    if (dats[k] >= mapLength){
      SendMediaKeys[k][0] = definedMediaKeys[dats[k] - mapLength][0];
      SendMediaKeys[k][1] = definedMediaKeys[dats[k] - mapLength][1];
    }else{
      symbols[k] = serverToHex[dats[k]].hexCode;
    }
  }
  //Sends an "empty" response back
  server.send(200, "text/html", "Hello world");
}
void handle_NotFound(){
  //Handles not found errors. 
  server.send(404, "text/plain", "Not found");
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
