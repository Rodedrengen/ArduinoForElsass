// include library to last and write from flash memory
#include <EEPROM.h>

// define the number of bytes you want to access
#define EEPROM_SIZE 32

bool commited = false;
bool last = false;

void setup()
{
  Serial.begin(115200);

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
}

void loop(){

  if (!last){

    int k = 0;
    while (k < EEPROM_SIZE){

      Serial.println(EEPROM.read(k));
      k++;
    }

    Serial.println("last");
    last = true;
  }
}
