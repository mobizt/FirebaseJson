# The Json Parser/Editor Arduino library.


The easiest Arduino library JSON parser, builder and editor.

FirebaseJson is the easiest JSON manipulation library to parse or deserialize complex or nested JSON objects and arrays. 

The new version of library is now powered by cJSON. 

Able to Parse, create and edit the simple or complex (depth nested) JSON object by just specify the relative node/element path.

This library supports any Arduino based MCU, ESP8266, ESP32, Teensy 3.x (ARM-Cortext M4) and Teensy 4.x (ARM-Cortext M7), AVR, SAMD, STM32 (128K flash or more) Arduino Nano RP2040 Connect, RaspberryPi Pico are recommended. 


## Tested Devices

 * ESP8266
 * ESP32
 * Teensy 3.6
 * Teensy 4.0
 * Teensy 4.1
 * Arduino MKR WiFi 1010
 * Arduino MKR1000 WIFI
 * STM32F103C
 * STM32FF407
 * RP2040 Pico


## Features


* **Non-recursive parsing**

* **Parse, Create and Edit JSON object directly by just specify the relative path to the node element**

* **Prettify JSON string**



## Dependencies


For ESP8266 and ESP32, the library requires **ESP8266 Core SDK version 2.4.0 and above or ESP32 Core SDK version 1.0.2 and above**.

To install the ESP8266 Core SDK from Arduino IDE, ESP8266/ESP32 Core SDK can be installed through **Boards Manager**. 

For PlatfoemIO IDE, ESP8266 Core SDK can be installed through **PIO Home** > **Platforms** > **Espressif 8266 or Espressif 32**.


For Teensy 3.x and Teensy 4.x, **[Teensyduino](https://www.pjrc.com/teensy/td_download.html)** was required and can be downloaded.



## Installation


For Arduino IDE, from Github repo, select **Clone or download** dropdown at the top of repository, select **Download ZIP** 

From Arduino IDE, select menu **Sketch** -> **Include Library** -> **Add .ZIP Library...**.

Choose **FirebaseJson-master.zip** that previously downloaded.

Go to menu **Files** -> **Examples** -> **FirebaseJson-master** and choose one from examples.


For PlatformIO IDE, using the following command.

pio lib install "FirebaseJson"

Or at PIO Home -> Library -> Registry then search FirebaseJson.



## IDE Configuaration for ESP8266 MMU - Adjust the Ratio of ICACHE to IRAM

### Arduino IDE

When you update the ESP8266 Arduino Core SDK to v3.0.0, the memory can be configurable from Arduino IDE board settings.

By default MMU **option 1** was selected, the free Heap can be low and may not suitable for the SSL client usage in this library.

To increase the Heap, choose the MMU **option 3**, 16KB cache + 48KB IRAM and 2nd Heap (shared).

![Arduino IDE config](/media/images/ArduinoIDE.png)

To use external Heap from 1 Mbit SRAM 23LC1024, choose the MMU **option 5**, 128K External 23LC1024.

![MMU VM 128K](/media/images/ESP8266_VM.png)

To use external Heap from PSRAM, choose the MMU **option 6**, 1M External 64 MBit PSRAM.

The connection between SRAM/PSRAM and ESP8266

```
23LC1024/ESP-PSRAM64                ESP8266

CS (Pin 1)                          GPIO15
SCK (Pin 6)                         GPIO14
MOSI (Pin 5)                        GPIO13
MISO (Pin 2)                        GPIO12
/HOLD (Pin 7 on 23LC1024 only)      3V3
Vcc (Pin 8)                         3V3
Vcc (Pin 4)                         GND
```

More about MMU settings.
https://arduino-esp8266.readthedocs.io/en/latest/mmu.html




### PlatformIO IDE

By default the balanced ratio (32KB cache + 32KB IRAM) configuration is used.

To increase the heap, **PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED** build flag should be assigned in platformio.ini.

```ini
[env:d1_mini]
platform = espressif8266
build_flags = -D PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
board = d1_mini
framework = arduino
monitor_speed = 115200
```

And to use external Heap from 1 Mbit SRAM 23LC1024 and 64 Mbit PSRAM, **PIO_FRAMEWORK_ARDUINO_MMU_EXTERNAL_128K** and **PIO_FRAMEWORK_ARDUINO_MMU_EXTERNAL_1024K** build flags should be assigned respectively.

The supportedd MMU build flags in PlatformIO.

- **PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48**

   16KB cache + 48KB IRAM (IRAM)

- **PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED**

   16KB cache + 48KB IRAM and 2nd Heap (shared)

- **PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM32_SECHEAP_NOTSHARED**

   16KB cache + 32KB IRAM + 16KB 2nd Heap (not shared)

- **PIO_FRAMEWORK_ARDUINO_MMU_EXTERNAL_128K**

   128K External 23LC1024

- **PIO_FRAMEWORK_ARDUINO_MMU_EXTERNAL_1024K**

   1M External 64 MBit PSRAM

- **PIO_FRAMEWORK_ARDUINO_MMU_CUSTOM**

   Disables default configuration and expects user-specified flags


In ESP8266, to use PSRAM/SRAM for internal memory allocation which you can config to use it via [**FBJS_Config.h**](src/FBJS_Config.h) with this macro.

```cpp
#define FIREBASEJSON_USE_PSRAM
```

   
### Test code for MMU

```cpp

#include <Arduino.h>
#include <umm_malloc/umm_heap_select.h>

void setup() 
{
  Serial.begin(115200);
  HeapSelectIram ephemeral;
  Serial.printf("IRAM free: %6d bytes\r\n", ESP.getFreeHeap());
  {
    HeapSelectDram ephemeral;
    Serial.printf("DRAM free: %6d bytes\r\n", ESP.getFreeHeap());
  }

  ESP.setExternalHeap();
  Serial.printf("External free: %d\n", ESP.getFreeHeap());
  ESP.resetHeap();
}

void loop() {
  // put your main code here, to run repeatedly:
}

```


## PSRAM support in ESP32

The library supports PSRAM in ESP32 via macro, in file **FBJS_Config.h**

```
#define FIREBASEJSON_USE_PSRAM
```

To enable PSRAM in ESP32 module with on-board PSRAM chip, in Arduino IDE

![Enable PSRAM in ESP32](/media/images/ESP32-PSRAM.png)


In PlatformIO in VSCode IDE, add the following build_flags in your project's platformio.ini file

```ini
build_flags = -DBOARD_HAS_PSRAM -mfix-esp32-psram-cache-issue
```

*When config the IDE or add the build flags to use PSRAM in the ESP32 dev boards that do not have on-board PSRAM chip, your device will be crashed (reset).


In ESP32, to use PSRAM/SRAM for internal memory allocation which you can config to use it via [**FBJS_Config.h**](src/FBJS_Config.h) with this macro.

```cpp
#define FIREBASEJSON_USE_PSRAM
```

## Usages


FirebaseJson usages are so simple as you read, store and update(edit) the JSON node in Firebase RTDB.

Since you declare the FirebaseJson or FirebaseJsonArray object, use the functions `setJsonData`, `setJsonArrayData`, `add`, `set` and `remove` to build or edit the JSON/Array object and use `get` to parse the node's contents. 

Defined the relative path of the specific node to `add`, `set`, `remove` and `get` functions to add, set, remove and get its contents.


Function `FirebaseJson.setJsonData` is to deserialize the JSON string to JSON object.

In addition, function `FirebaseJson.readFrom` can be used to read the streaming JSON contents from WiFi/Ethernet Client, File and Harware Serial and serialize it as the streaming content contains valid JSON data. 


Function `FirebaseJson.add` is used to add the new node with the contents e.g. String, Number (int and double), Boolean, Array and Object to the defined node.


Function `FirebaseJson.set` is used for edit, overwrite, create new (if not exist) node with contents e.g. String, Number (int and double), Boolean, Array and Object at the defined relative path and node.


Function `FirebaseJson.get` is used for parsing or deserializee the JSON object and array. The deserialized or parsed result will keep in FirebaseJsonData object which can be casted to any type of value or variable e.g string, bool, int, float, double by using `FirebaseJsonData.to<type>`. 

The casting from FirebaseJsonData to FirebaseJson and FirebaseJsonArray objects is different, by using `FirebaseJsonData.getJSON(FirebaseJson)` and `FirebaseJsonData.getArray(FirebaseJsonArray)`.


Function `FirebaseJson.remove` is used to remove the node and all its children's contents at the defined relative path and node. 


Function `FirebaseJson.toString` is used for serializeing the JSON object to writable objects e.g. char array, Arduino String, C/C++ string, WiFi/Ethernet Client and Hardware/Software Serial.


Function `FirebaseJson.serializedBufferLength` is used for calculating the serialized buffer size that required for reserved buffer in serialization.


Function `FirebaseJson.responseCode` is used to get the http code response header while read the WiFi/Ethernet Client using `FirebaseJson.toString`.


Functions `FirebaseJson.iteratorBegin`, `FirebaseJson.iteratorGet` and `FirebaseJson.iteratorEnd` are used to parse all JSON object contents as a list which can be iterated with index.


Function `FirebaseJson.clear` is used to clear JSON object contents.


Function `FirebaseJson.setFloatDigits` is for float number precision when serialized to string.


Function `FirebaseJson.setDoubleDigits` is for double number precision when serialized to string.


Function `FirebaseJsonArray.add` is used for adding the new contents e.g. String, Number (int and double), Boolean, Array and Object to JSON array.


Function `FirebaseJsonArray.set` is for edit, overwrite, create new (if not exist) contents e.g. String, Number (int and double), Boolean, Array and Object at the defined relative path or defined index of JSON array.


Function `FirebaseJsonArray.get` works in the same way as FirebaseJson objects


Function `FirebaseJsonArray.remove` is used to remove the array's contents at the defined relative path or defined index of JSON array.


Function `FirebaseJsonArray.toString` is used for serializeing the JSON array object to writable objects e.g. char array, Arduino String, C/C++ string, WiFi/Ethernet Client and Hardware/Software Serial.


Function `FirebaseJsonArray.serializedBufferLength` is used for calculating the serialized buffer size that required for reserved buffer in serialization.


Function `FirebaseJsonArray.responseCode` is used to get the http code response header while read the WiFi/Ethernet Client using `FirebaseJson.toString`.


Function `FirebaseJsonArray.clear` is used to clear JSON array object contents.


Function `FirebaseJsonArray.setFloatDigits` is for float number precision when serialized to string.


Function `FirebaseJsonArray.setDoubleDigits` is for double number precision when serialized to string.



The following example shows how to use FirebaseJson.

```C++
//Declare FirebaseJson object (global or local)
FirebaseJson json;

//Add name with value Living Room to JSON object
json.add("name", "Living Room");

//Add temp1 with value 120 and temp1 with 40 to JSON object
//Note: temp2 is not the child of temp1 as in previous version.
json.add("temp1", 120).add("temp2", 40);

//Add nested child contents directly
json.set("unit/temp1", "Farenheit");
json.set("unit/temp2", "Celcius");

//Deserialize to serial with prettify option
json.toString(Serial, true);
Serial.println();
Serial.println();

/**
This is the result of the above code

{
    "name": "Living Room",
    "temp1": 120,
    "temp2": 40,
    "unit": {
        "temp1": "Farenheit",
        "temp2": "Celcius"
    }
}
*/

//To set array to the above JSON using FirebaseJson directly
//Set (add) array indexes 0,1,2,5,7 under temp1, the original value will be replaced with new one.
json.set("temp1/[0]", 47);
json.set("temp1/[1]", 28);
json.set("temp1/[2]", 34);
json.set("temp1/[5]", 23); //null will be created at array index 3,4 due to it's not yet assigned
json.set("temp1/[7]", 25); //null will be created at array index 6

//Print out as prettify string
json.toString(Serial, true);
Serial.println();
Serial.println();

/**
The result of the above code

{
    "name": "Living Room",
    "temp1": [
        47,
        28,
        34,
        null,
        null,
         23,
        null,
        25
     ],
    "temp2": 40,
    "unit": {
        "temp1": "Farenheit",
        "temp2": "Celcius"
    }
 }
*/

//Try to remove temp1 array at index 1
json.remove("temp1/[1]");

//Try to remove temp2
json.remove("temp2");

//Print out as prettify string
json.toString(Serial, true);
Serial.println();
Serial.println();

/**
The result of the above code

{
    "name": "Living Room",
    "temp1": [
         47,
         34,
         null,
         null,
         23,
         null,
         25
    ],
    "unit": {
        "temp1": "Farenheit",
        "temp2": "Celcius"
    }
}
*/

//Now parse/read the contents from specific node unit/temp2
//FirebaseJsonData is required to keep the parse results which can be accessed later
FirebaseJsonData result;

json.get(result, "unit/temp2");

if (result.success)
{
  //Print type of parsed data e.g string, int, double, bool, object, array, null and undefined
  Serial.println(result.type);
  //Print its content e.g.string, int, double, bool whereas object, array and null also can access as string
  Serial.println(result.to<String>());
  //Serial.println(result.to<int>());
  //Serial.println(result.to<bool>());
  //Serial.println(result.to<float>());
  //Serial.println(result.to<double>());
}

//The above code will show
/**
string
Celcius
*/

//To get the array temp from FirebaseJson

json.get(result, "temp1");

//Prepare FirebaseJsonArray to take the array from FirebaseJson
FirebaseJsonArray arr;

//Get array data
result.get<FirebaseJsonArray>(arr);

//Call get with FirebaseJsonData to parse the array at defined index i
for (size_t i = 0; i < arr.size(); i++)
{
  //result now used as temporary object to get the parse results
  arr.get(result, i);

  //Print its value
  Serial.print("Array index: ");
  Serial.print(i);
  Serial.print(", type: ");
  Serial.print(result.type);
  Serial.print(", value: ");
  Serial.println(result.to<String>());
}

/**
The result of above code
Array index: 0, type: int, value: 47
Array index: 1, type: int, value: 34
Array index: 2, type: null, value: null
Array index: 3, type: null, value: null
Array index: 4, type: int, value: 23
Array index: 5, type: null, value: null
Array index: 6, type: int, value: 25
*/
 

```


The following example shows how to use FirebaseJsonArray.

```C++
//Declare FirebaseJsonArray object (global or local)
FirebaseJsonArray arr;

//Add some data
arr.add("banana");
arr.add("mango");
arr.add("coconut");


//Change the array contents
arr.set("[1]/food", "salad");
arr.set("[1]/sweet", "cake");
arr.set("[1]/appetizer", "snack");
arr.set("[2]", "apple"); // or arr.set(2, "apple");
arr.set("[4]/[0]/[1]/amount", 20);

//Print out array as prettify string
arr.toString(Serial, true);
Serial.println();
Serial.println();

/**
This is the result of the above code

[
    "banana",
    {
        "food": "salad",
        "sweet": "cake",
        "appetizer": "snack"
    },
    "apple",
    null,
    [
        [
            null,
            {
                "amount": 20
            }
        ]
    ]
]
*/

//Remove array content at /4/0/1/amount
arr.remove("[4]/[0]/[1]/amount");

//Print out as prettify string
arr.toString(Serial, true);
Serial.println();
Serial.println();
/**
The result of the above code

[
    "banana",
    {
        "food": "salad",
        "sweet": "cake",
        "appetizer": "snack"
    },
    "apple",
    null,
    [
        [
            null
        ]
    ]
]

*/

//Now parse/read the array contents at some index

FirebaseJsonData result;

arr.get(result, "[1]/food");

if(result.success)
{
  //Type of parsed data
  Serial.println(result.type);
  //Its value
  Serial.println(result.stringValue);
  //Serial.println(result.intValue);
  //Serial.println(result.boolValue);
  //Serial.println(result.floatValue);
  //Serial.println(result.doubleValue);

}

//The above code will show
/**
string
salad
*/


//To get the JSON object at array index 1 from FirebaseJsonArray
arr.get(result, "[1]");// or arr.get(result, 1);

//Prepare FirebaseJson to take the JSON object from FirebaseJsonArray
FirebaseJson json;

//Get FirebaseJson data
result.get<FirebaseJson>(json);

//Parse the JSON object as list
//Get the number of items
size_t len = json.iteratorBegin();
FirebaseJson::IteratorValue value;
for (size_t i = 0; i < len; i++)
{
    value = json.valueAt(i);
    Serial.printf("%d, Type: %s, Name: %s, Value: %s\n", i, value.type == FirebaseJson::JSON_OBJECT ? "object" : "array", value.key.c_str(), value.value.c_str());
}

//Clear all list to free memory
json.iteratorEnd();


/**
The result of the above code

0, Type: object, Key: food, Value: salad
1, Type: object, Key: sweet, Value: cake
2, Type: object, Key: appetizer, Value: snack

*/


```



## All Available Functions



## FirebaseJSON object Functions


#### Set or deserialize the JSON object data (JSON object literal) as FirebaseJson object.

param **`data`** The JSON object literal string to set or deserialize.

return **`bool`** value represents the successful operation.

Call FirebaseJson.errorPosition to get the error.

```C++
bool setJsonData(<string> data);
```







#### Clear internal buffer of FirebaseJson object.
    
return **`instance of an object.`**

```C++
FirebaseJson &clear();
```






#### Set JSON data via derived Stream object to FirebaseJson object.
    
param **`stream`** The pointer to or instance of derived Stream class.

return **`boolean`** status of the operation.

```C++
bool readFrom(Stream &stream);

bool readFrom(Stream *stream);
```






#### Set JSON data via derived Client object to FirebaseJson object.
    
param **`client`** The pointer to or instance of derived Client class.

return **`boolean`** status of the operation.

```C++
bool readFrom(Client &client);

bool readFrom(Client *client);
```





#### Set JSON data via Serial to FirebaseJson object.
    
param **`ser`** The Serial object.

param **`ser`** The timeout in millisecond to wait for Serial data to be completed.

return **`boolean`** status of the operation.

```C++
bool readFrom(<serial> &ser, uint32_t timeoutMS = 5000) ;
```






#### Set JSON data via SdFat's SdFile object to FirebaseJson object.
    
param **`sdFatFile`** The SdFat file object.

return **`boolean`** status of the operation.

```C++
bool readFrom(<SdFatFile> &sdFatFile);
```







#### Add null to FirebaseJson object.
    
param **`key`** The new key string that null to be added.

return **`instance of an object.`**

```C++
FirebaseJson &add(<string> key);
```






#### Add value to FirebaseJson object.
    
param **`key`** The new key string that string value to be added.

param **`value`** The value for the new specified key.

return **`instance of an object.`**

The value that can be added is the following supported types e.g. flash string (PROGMEM and FPSTR), String, C/C++ std::string, const char*, char array, string literal, all integer and floating point numbers, boolean, FirebaseJson object and array.

```C++
FirebaseJson &add(<string> key, <type> value);
```








#### Get the FirebaseJson object serialized string.

param **`out`** The writable object e.g. String, std::string, char array, Stream e.g ile, WiFi/Ethernet Client and LWMQTT, that accepts the returning string.

param **`topic`** The MQTT topic (LWMQTT).

param **`prettify`** Boolean flag for return the pretty format string i.e. with text indentation and newline. 

```C++
void toString(<type> out, bool prettify = false);

void toString(<type> out, <string> topic, bool prettify = false);
```








#### Get the value from the specified node path in FirebaseJson object.

param **`result`** The reference of FirebaseJsonData that holds the result.

param **`path`** Relative path to the specific node in FirebaseJson object.

param **`prettify`** The text indentation and new line serialization option.

return **`boolean status of the operation.`**

The FirebaseJsonData object holds the returned data which can be read from the following properties.

jsonData.stringValue - contains the returned string.

jsonData.intValue - contains the returned integer value.

jsonData.floatValue - contains the returned float value.

jsonData.doubleValue - contains the returned double value.

jsonData.boolValue - contains the returned boolean value.

jsonData.success - used to determine the result of the get operation.

jsonData.type - used to determine the type of returned value in string represents 
the types of value e.g. string, int, double, boolean, array, object, null and undefined.

jsonData.typeNum used to determine the type of returned value is an integer as represented by the following value.
    
FirebaseJson::UNDEFINED = 0

FirebaseJson::OBJECT = 1

FirebaseJson::ARRAY = 2

FirebaseJson::STRING = 3

FirebaseJson::INT = 4

FirebaseJson::FLOAT = 5

FirebaseJson::DOUBLE = 6

FirebaseJson::BOOL = 7 and

FirebaseJson::NULL = 8
 
 ```C++
 bool get(FirebaseJsonData &result, <string> path, bool prettify = false);
 ```





#### Parse and collect all node/array elements in FirebaseJson object.

return **`number`** of child/array elements in FirebaseJson object.

 ```C++
 size_t iteratorBegin();
 ```






#### Get child/array elements from FirebaseJson objects at specified index.
    
param **`index`** The element index to get.

param **`type`** The integer which holds the type of data i.e. FirebaseJson::OBJECT and FirebaseJson::ARRAY

param **`key`** The string which holds the key/name of the object, can return empty String if the data type is an array.

param **`value`** The string which holds the value for the element key or array.   

 ```C++
 void iteratorGet(size_t index, int &type, String &key, String &value);
 ```








#### Get child/array elements from FirebaseJson objects at specified index.

param **`index`** The element index to get.   

return **` IteratorValue struct`** 

This should call after iteratorBegin.

The IteratorValue struct contains the following members:
int type
String key
String value

 ```C++
 IteratorValue valueAt(size_t index);
 ```







#### Clear all iterator buffer (should be called since iteratorBegin was called).

 ```C++
 void iteratorEnd();
 ```





#### Set null to FirebaseJson object at the specified node path.
    
param **`path`** The relative path that null to be set.


The relative path can be mixed with array index (number placed inside square brackets) and node names e.g. /myRoot/[2]/Sensor1/myData/[3].


```C++
void set(<string> path);
```





#### Set value to FirebaseJson object at the specified node path.
    
param **`path`** The relative path that string value to be set.

param **`value`** The value to set.


The relative path can be mixed with array index (number placed inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

The value that can be added is the following supported types e.g. flash string (PROGMEM and FPSTR), String, C/C++ std::string, const char*, char array, string literal, all integer and floating point numbers, boolean, FirebaseJson object and array.

```C++
void set(<string> path, <type> value);
```







#### Remove the specified node and its content.

param **`path`** The relative path to remove its contents/children.

return **`bool`** value represents the successful operation.

```C++
bool remove(<string> path);
```






#### Get the error position at the JSON object literal from parsing.

return **`the position of error in JSON object literal`**

Return -1 when for no parsing error.

```C++
int errorPosition();
```







#### Get the size of serialized JSON object buffer.

param **`prettify`** The text indentation and new line serialization option.

return **`size in byte of buffer`**

```C++
size_t serializedBufferLength(bool prettify = false);
```







#### Set the precision for float to JSON object.

param **`digits`** The number of decimal places.

```C++
void setFloatDigits(uint8_t digits);
```






#### Set the precision for double to JSON object.

param **`digits`** The number of decimal places.

```C++
void setDoubleDigits(uint8_t digits);
```






#### Get http response code of reading JSON data from WiFi/Ethernet Client.

return **`the response code`** of reading JSON data from WiFi/Ethernet Client 

```C++
int responseCode();
```







### FirebaseJsonArray object functions



#### Set or deserialize the JSON array data (JSON array literal) as FirebaseJsonArray object.

param **`data`** The JSON array literal string to set or deserialize.

return **`bool`** value represents the successful operation.

Call FirebaseJsonArray.errorPosition to get the error.

```C++
bool setJsonArrayData(<string> data);
```







#### Add null to FirebaseJsonArray object.

return **`instance of an object.`**

```C++
FirebaseJsonArray &add();
```






#### Add value to FirebaseJsonArray object.

param **`value`** The value to add.

return **`instance of an object.`**

The value that can be added is the following supported types e.g. flash string (PROGMEM and FPSTR), String, C/C++ std::string, const char*, char array, string literal, all integer and floating point numbers, boolean, FirebaseJson object and array.


```C++
FirebaseJsonArray &add(<type> value);
```






#### Add multiple values to FirebaseJsonArray object.

param **`v`** The value of any type to add.

param **`n`** The consecutive values of any type to add.

return **`instance of an object.`**

e.g. add("a","b",1,2)

```C++
FirebaseJsonArray &add(First v, Next... n);
```







#### Set JSON array data via derived Stream object to FirebaseJsonArray object.
    
param **`stream`** The pointer to or instance of derived Stream class.

return **`boolean`** status of the operation.

```C++
bool readFrom(Stream &stream);

bool readFrom(Stream *stream);
```








#### Set JSON data array via derived Client object to FirebaseJsonArray object.
    
param **`client`** The pointer to or instance of derived Client class.

return **`boolean`** status of the operation.

```C++
bool readFrom(Client &client);

bool readFrom(Client *client);
```






#### Set JSON array data via Serial to FirebaseJsonArray object.
    
param **`ser`** The Serial object.

param **`ser`** The timeout in millisecond to wait for Serial data to be completed.

return **`boolean`** status of the operation.

```C++
bool readFrom(<serial> &ser, uint32_t timeoutMS = 5000) ;
```







#### Set JSON array data via SdFat's SdFile object to FirebaseJsonArray object.
    
param **`sdFatFile`** The SdFat file object.

return **`boolean`** status of the operation.

```C++
bool readFrom(<SdFatFile> &sdFatFile);
```








#### Get the array value at the specified index or path from the FirebaseJsonArray object.

param **`result`** The reference of FirebaseJsonData object that holds data at the specified index.

param **`index_or_path`** Index of data or relative path to data in FirebaseJsonArray object.    

return **`boolean`** status of the operation.

The relative path must begin with array index (number placed inside square brackets) followed by
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2

```C++
bool get(FirebaseJsonData &result, <int or string> index_or_path);
```







#### Parse and collect all node/array elements in FirebaseJsonArray object.

return **`number`** of child/array elements in FirebaseJsonArray object.

 ```C++
 size_t iteratorBegin();
 ```






#### Get child/array elements from FirebaseJsonArray objects at specified index.
    
param **`index`** The element index to get.

param **`type`** The integer which holds the type of data i.e. FirebaseJson::OBJECT and FirebaseJson::ARRAY

param **`key`** The string which holds the key/name of the object, can return empty String if the data type is an array.

param **`value`** The string which holds the value for the element key or array.   

 ```C++
 void iteratorGet(size_t index, int &type, String &key, String &value);
 ```








#### Get child/array elements from FirebaseJsonArray objects at specified index.

param **`index`** The element index to get.   

return **` IteratorValue struct`** 

This should call after iteratorBegin.

The IteratorValue struct contains the following members:
int type
String key
String value

 ```C++
 IteratorValue valueAt(size_t index);
 ```







#### Clear all iterator buffer (should be called since iteratorBegin was called).

 ```C++
 void iteratorEnd();
 ```






#### Get the length of array in FirebaseJsonArray object.  

return **`length of the array.`**

```C++
size_t size();
```








#### Get the FirebaseJsonArray object serialized string.

param **`out`** The writable object e.g. String, std::string, char array, Stream e.g ile, WiFi/Ethernet Client and LWMQTT, that accepts the returning string.

param **`prettify`** Boolean flag for return the pretty format string i.e. with text indentation and newline. 

```C++
void toString(<type> out, bool prettify = false);
```




#### Get raw JSON Array.

return **`raw JSON Array string`**

```C++
const char *raw();
```







#### Get the size of serialized JSON array buffer.

param **`prettify`** The text indentation and new line serialization option.

return **`size in byte of buffer`**

```C++
size_t serializedBufferLength(bool prettify = false);
```






#### Clear all array in FirebaseJsonArray object.

return **`instance of an object.`**

```C++
FirebaseJsonArray &clear();
```







#### Set null to FirebaseJsonArray object at at specified index or path.
    
param **`index_or_path`** The array index or path that null to be set.

```C++
void set(<int or string> index_or_path);
```







####  Set String to FirebaseJsonArray object at the specified index.
    
param **`index_or_path`** The array index or path that value to be set.

param **`value`** The value to set.


```C++
void set(<int or string> index_or_path, <type> value);
```





#### Remove the array value at the specified index or path from the FirebaseJsonArray object.

param **`index_or_path`** The array index or relative path to array to be removed.

return **`bool`** value represents the successful operation.

```C++
bool remove(<int or string> index_or_path);
```





#### Get raw JSON.

return **`raw JSON string`**

```C++
const char *raw();
```






#### Get the error position at the JSON array literal from parsing.

return **`the position of error in JSON array literal`**

Return -1 when for no parsing error.

```C++
int errorPosition();
```






#### Set the precision for float to JSON Array object.

param **`digits`** The number of decimal places.

```C++
void setFloatDigits(uint8_t digits);
```






#### Set the precision for double to JSON Array object.

param **`digits`** The number of decimal places.

```C++
void setDoubleDigits(uint8_t digits);
```




### FirebaseJsonData object functions


#### Get array data as FirebaseJsonArray object from FirebaseJsonData object.
    
param **`jsonArray`** The returning FirebaseJsonArray object.

return **`bool`** status for successful operation.

This should call after pares or get functions.

```C++
bool getArray(FirebaseJsonArray &jsonArray);
```







#### Get array data as FirebaseJsonArray object from FirebaseJsonData object.

param **`source`** The JSON array string.
    
param **`jsonArray`** The returning FirebaseJsonArray object.

return **`bool`** status for successful operation.

This should call after pares or get functions.

```C++
bool getArray(<string> source, FirebaseJsonArray &jsonArray);
```







#### Get array data as FirebaseJson object from FirebaseJsonData object.
    
param **`jsonArray`** The returning FirebaseJson object.

return **`bool`** status for successful operation.

This should call after pares or get functions.

```C++
bool getJSON(FirebaseJson &json);
```






#### Get JSON data as FirebaseJson object from string.

param **`source`** The JSON string.
    
param **`json`** The returning FirebaseJsonArray object.

return **`bool`** status for successful operation.

This should call after pares or get functions.

```C++
bool getJSON(<string> source, FirebaseJson &json);
```







 #### Cast the FirebaseJsonData object to object or primitive type variable.

 return the **`The object or primitive type variable`**.

 ```C++
to<type>();

e.g. to<String>(), to<int>(), to<bool>()
```



### FirebaseJsonData object properties


**`stringValue`** The String value of parses data.

**`intValue`** The int value of parses data.

**`doubleValue`** The double value of parses data.

**`floatValue`** The float value of parses data.

**`boolVlue`** The bool value of parses data.

**`success`** used to determine the result of the deserialize operation.

**`type`** The type String of parses data e.g. string, int, double, boolean, array, object, null and undefined.

**`typeNum`** The type (number) of parses data in form of the following **`jsonDataType`** value.

**FirebaseJson::UNDEFINED = 0**

**FirebaseJson::OBJECT = 1**

**FirebaseJson::ARRAY = 2**

**FirebaseJson::STRING = 3**

**FirebaseJson::INT = 4**

**FirebaseJson::FLOAT = 5**

**FirebaseJson::DOUBLE = 6**

**FirebaseJson::BOOL = 7 and**

**FirebaseJson::NULL = 8**





## License

The MIT License (MIT)

Copyright (c) 2023 K. Suwatchai (Mobizt)

Copyright (c) 2009-2017 Dave Gamble and cJSON contributors


Permission is hereby granted, free of charge, to any person returning a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

