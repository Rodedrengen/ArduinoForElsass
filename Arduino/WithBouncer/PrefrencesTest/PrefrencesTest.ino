#include <Preferences.h>

Preferences preferences;

const char *liste[8]{
    "stringDat0",
    "stringDat1",
    "stringDat2",
    "stringDat3",
    "stringDat4",
    "stringDat5",
    "stringDat6",
    "stringDat7"};

void setup()
{

    Serial.begin(115200);
    Serial.println();

    preferences.begin("my-app", false);
    for (int i = 0; i < 8; i++)
    {
        preferences.getInt(liste[i], 0);
    }

    for (int k = 0; k < 8; k++)
    {
        Serial.println(liste[k]);
       preferences.putInt(liste[k], k);
    }

    for (int j = 0; j < 8; j++)
    {
        Serial.println(preferences.getInt(liste[j]));
    }
    

    // Close the Preferences
    preferences.end();

    // Wait 10 seconds
    Serial.println("Restarting in 10 seconds...");
    delay(10000);

    // Restart ESP
}

void loop()
{
}