/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

#include <Arduino.h>
#include <FirebaseJson.h>

FirebaseJson json;

#ifndef DEFAULT_FLASH_FS
#include <FS.h>
#include <SPIFFS.h>
#define DEFAULT_FLASH_FS SPIFFS
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println();

    if (!DEFAULT_FLASH_FS.begin())
    {
        Serial.println("SPIFFS/LittleFS initialization failed.");
        Serial.println("For Arduino IDE, please select flash size from menu Tools > Flash size");
        return;
    }

    //Delete demo files
    if (DEFAULT_FLASH_FS.exists("/test.txt"))
        DEFAULT_FLASH_FS.remove("/test.txt");

    if (DEFAULT_FLASH_FS.exists("/test2.txt"))
        DEFAULT_FLASH_FS.remove("/test2.txt");

    //Write demo data to file
    File file = DEFAULT_FLASH_FS.open("/test.txt", "w");

    //Print the demo data to file.
    //No matter what the separation between each JSON object or array is, it can be space, new line or nothing
    file.print("{\"a\":123}{\"b\":456}{\"c\":789}");

    file.close();
}

void loop()
{
    File file = DEFAULT_FLASH_FS.open("/test.txt", "r");
    while (file.available())
    {
        //No matter what JSON data separation is newline or any.
        if (json.readFrom(file))
        {
            Serial.println("JSON Data read...");
            json.toString(Serial, true);
            Serial.println();

            Serial.println();
            Serial.println("Appended data to file...");
            Serial.println();

            //Append json data to another file
            File file2 = DEFAULT_FLASH_FS.open("/test2.txt", "a");
            json.toString(file2);

            //if you want to append new line to separate each JSON data (not necessary)
            //file2.println();

            file2.close();
        }
    }
    file.close();

    Serial.println("Read appended data file...");
    file = DEFAULT_FLASH_FS.open("/test2.txt", "r");
    while (file.available())
    {
        Serial.print((char)file.read());
    }
    Serial.println();
    file.close();

    if (DEFAULT_FLASH_FS.exists("/test2.txt"))
        DEFAULT_FLASH_FS.remove("/test2.txt");

    delay(15000);
}