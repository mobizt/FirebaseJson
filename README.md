# The Firebase Json Arduino library for ESP8266/ESP32.


This Arduino library is for create, edit and get the value from JSON object in simple way which best for Firebase v 2.0.0


This library supports ESP8266 and ESP32 from Espressif. 


## Tested Devices

 * ESP8266
 * ESP32


## Features


* **Fast and Simple JSON Builder, Editor and Parser.**

* **Firebase JSON compatible.**

* **Get and Set and Update nested JSON object or nested Array directly**



## Dependencies


This library required **ESP8266 Core SDK version 2.4.0 and above or ESP32 Core SDK version 1.0.2 and above**.

For Arduino IDE, ESP8266/ESP32 Core SDK can be installed through **Boards Manager**. 

For PlatfoemIO IDE, ESP8266 Core SDK can be installed through **PIO Home** > **Platforms** > **Espressif 8266 or Espressif 32**.




## Installation


For Arduino IDE, click on **Clone or download** dropdown at the top of repository, select **Download ZIP** 

From Arduino IDE, goto menu **Sketch** -> **Include Library** -> **Add .ZIP Library...**.

Choose **FirebaseJson-master.zip** that previously downloaded.

Go to menu **Files** -> **Examples** -> **FirebaseJson-master** and choose one from examples.


For PlatformIO, create folder **"FirebaseJson"** in folder **"lib"** and save **[these files](https://github.com/mobizt/FirebaseJson/tree/master/src)** in there.






## Usages


### See the examples for usages


## All Available Functions

### FirebaseJson object functions


#### Clear buffer and tokens pointer of FirebaseJson object.
    
return **`instance of an object.`**

```C++
FirebaseJson &clear();
```






#### Set JSON data (string) to FirebaseJson object.
    
param **`data`** - The JSON object string.

return **`instance of an object.`**

```C++
FirebaseJson &setJsonData(const String &data);
```






#### Add null to FirebaseJson object.
    
param **`key`** - The new key string to add null.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key);
```






#### Add string to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`value`** - The String value for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, const String &value);
```






#### Add string to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`value`** - The char array for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, const char *value);
```






#### Add integer to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`value`** - The integer value for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, int value);
```






#### Add double to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`value`** - The double value for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, double value);
```





#### Add boolean to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`value`** - The boolean value for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, bool value);
```





#### Add nested FirebaseJson object to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`json`** - The FirebaseJson object for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, FirebaseJson &json);
```






#### Add nested FirebaseJsonArray object to FirebaseJson object.
    
param **`key`** - The new key string to add.

param **`arr`** - The FirebaseJsonArray for new specified key.

return **`instance of an object.`**

```C++
FirebaseJson &add(const String &key, FirebaseJsonArray &arr);
```






#### Get the FirebaseJson object serialized string.

param **`prettify`** - Boolean flag for return the pretty format string i.e. with text indent and newline. 

return **`serialized string of JSON object.`**

```C++
String toString(bool prettify = false);
```






#### Get the value from specified node path in FirebaseJson object.

param **`jsonData`** - The FirebaseJsonData that hold the returned data.

param **`path`** - Relative path to the node in FirebaseJson object.  

return **`boolean status of operation.`**

    The FirebaseJsonData object hold the returned data with the following properties
    jsonData.stringValue, jsonData.intValue, jsonData.doubleValue, jsonData.boolValue and
    jsonData.success, jsonData.type and jsonData.typeNum.

    jsonData.stringValue contains the returned string.

    jsonData.intValue contains the returned integer value.

    jsonData.doubleValue contains the returned double value.

    jsonData.boolValue contains the returned boolean value.

    jsonData.success used to determine the result of get operation.

    jsonData.type used to determine the type of returned value in string represent 
    the types of value e.g. string, int, double, boolean, array, object, null and undefined.

    jsonData.typeNum used to determine the type of returned value in integer as represented by the following value.
    
    JSON_UNDEFINED = 0
    JSON_OBJECT = 1
    JSON_ARRAY = 2
    JSON_STRING = 3
    JSON_INT = 4
    JSON_DOUBLE = 5
    JSON_BOOL = 6 and
    JSON_NULL = 7

 
 ```C++
 bool get(FirebaseJsonData &jsonData, const String &path);

#### Get the numbers of JSON tokens (keys + values + 1) to be available for read from jsonObjectIterator.  

return **`numbers of tokens.`**

 ```C++
 size_t getJsonObjectIteratorCount();
 ```






#### Parse FirebaseJson object.

return **`instance of an object.`**

```C++
FirebaseJson &parse();
 ```






#### Read data from JSON objects by providing tokens ID.
    
param **`index`** - The referenced token index. This will auto increase to the next token index after read.

param **`key`** - The referenced key data string. This provided the key data output.

param **`value`** - The referenced value string. This provided the value of current key output.   

return **`instance of an object.`**


```C++
FirebaseJson &jsonObjectiterator(size_t &index, String &key, String &value);
```






#### Get the parse result of JSON object.   

return **`FirebaseJsonData.`**

FirebaseJsonData holds the return data as in get function above


```C++
FirebaseJsonData parseResult();
```






 #### Set null to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set null.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].


```C++
bool set(const String &path);
```





#### Set String value to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set value.

param **`value`** - The string value to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

```C++
bool set(const String &path, const String &value);
```





#### Set String value to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set value.

param **`value`** - The char array to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

```C++
bool set(const String &path, const char *value);
```





#### Set integer value to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set value.

param **`value`** - The integer value to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

```C++
bool set(const String &path, int value);
```







 #### Set double value to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set value.

param **`value`** - The double value to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

```C++
bool set(const String &path, double value);
```







#### Set boolean value to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set value.

param **`value`** - The boolean value to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

```C++
bool set(const String &path, bool value);
```







#### Set nested FirebaseJson object to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set the nested FirebaseJson object.

param **`json`** - The FirebaseJson object to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

 ```C++
bool set(const String &path, FirebaseJson &json);
```







#### Set nested FirebaseJsonAtrray object to FirebaseJson object at specified node path.
    
param **`path`** - The relative path to set the nested FirebaseJsonAtrray object.

param **`arr`** - The FirebaseJsonAtrray object to set.

return **`bool`** value represents the success operation.

The relative path can be mixed with array index (index number of array inside square brackets) and node names 
e.g. /myRoot/[2]/Sensor1/myData/[3].

```C++
bool set(const String &path, FirebaseJsonArray &arr);
```






#### Remove specified node and its content.

param **`path`** - The relative path to remove content and children.

return **`bool`** value represents the success operation.

```C++
bool remove(const String &path);
```



### FirebaseJsonArray object functions





#### Get the array value at specified index from FirebaseJsonArray object.

param **`jsonObj`** - FirebaseJsonData object that holds data at specified index returned FirebaseJsonArray object.

param **`index`** - Index of data in FirebaseJsonArray object.    

return **`boolean`** status of operation.

```C++
bool get(FirebaseJsonData &jsonObj, int index);
```







#### Get the array value at specified path from FirebaseJsonArray object.

param **`jsonObj`** - FirebaseJsonData object that holds data at specified path returned FirebaseJsonArray object.

param **`path`** - Relative path to data in FirebaseJsonArray object.    

return **`boolean status of operation.`**

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2

```C++
bool get(FirebaseJsonData &jsonObj, const String &path);
```






#### Get the length of array in FirebaseJsonArray object.  

return **`length of array.`**

```C++
size_t size();
```






#### Get the FirebaseJsonArray object serialized string.

param **`prettify`** - Boolean flag for return the pretty format string i.e. with text indent and newline. 

return **`serialized string of JSON Array object.`**

```C++
String toString(bool prettify = false);
```






#### Clear array in FirebaseJsonArray object.

return **`instance of an object.`**

```C++
FirebaseJsonArray &clear();
```







#### Set null to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set null.

return **`bool`** value represents the success operation.

```C++
bool set(int index);
```







#### Set String to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set value.

param **`value`** - The String to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, const String &value);
```







#### Set String to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set value.

param **`value`** - The char array to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, const char *value);
```





#### Set integer value to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set value.

param **`value`** - The integer value to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, int value);
```






#### Set double value to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set value.

param **`value`** - The double value to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, double value);
```






#### Set boolean value to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set value.

param **`value`** - The boolean value to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, bool value);
```






#### Set nested FirebaseJson object to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set nested FirebaseJson object.

param **`value`** - The FirebaseJson object to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, FirebaseJson &json);
```






#### Set nested FirebaseJsonArray object to FirebaseJsonArray object at specified index.
    
param **`index`** - The array index to set nested FirebaseJsonArray object.

param **`value`** - The FirebaseJsonArray object to set.

return **`bool`** value represents the success operation.

```C++
bool set(int index, FirebaseJsonArray &arr);
```






    
#### Set null to FirebaseJson object at specified path.
    
param **`path`** - The relative path to set null.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path);
```





#### Set String to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The String to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path, const String &value);
```




#### Set string to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The char array to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path, const char *value);
```





#### Set integer value to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The integer value to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path, int value);
```





#### Set double value to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The double to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

 ```C++
bool set(const String &path, double value);
```





#### Set boolean value to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The boolean value to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path, bool value);
```





#### Set the nested FirebaseJson object to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The FirebaseJson object to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path, FirebaseJson &json);
```






#### Set the nested FirebaseJsonArray object to FirebaseJsonArray object at specified path.
    
param **`path`** - The relative path to set value.

param **`value`** - The FirebaseJsonArray object to set.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

```C++
bool set(const String &path, FirebaseJsonArray &arr);
```






#### Remove the array value at specified index from FirebaseJsonArray object.

param **`index`** - The array index to remove.

return **`bool`** value represents the success operation.

```C++
bool remove(int index);
```






#### Remove the array value at specified path from FirebaseJsonArray object.

param **`path`** - The relative path to array in FirebaseJsonArray object to remove.

return **`bool`** value represents the success operation.

The relative path must be begin with array index (index number of array inside square brackets) followed by 
other array indexes or node names e.g. /[2]/myData would remove the data of myData key inside the array indexes 2.
    
```C++
bool remove(const String &path);
```





### FirebaseJsonData object functions


#### Get array data as FirebaseJsonArray object from FirebaseJsonData object.
    
param **`jsonArray`** - The FirebaseJsonArray object to get.

return **`bool`** status for success operation.

This should call after pares or get function.

```C++
bool getArray(FirebaseJsonArray &jsonArray);
```





#### Get array data as FirebaseJson object from FirebaseJsonData object.
    
param **`jsonArray`** - The FirebaseJson object to get.

return **`bool`** status for success operation.

This should call after pares or get function.

```C++
bool getJSON(FirebaseJson &json);
```



### FirebaseJsonData object properties


**`stringValue`** contains the returned string.

**`intValue`** contains the returned integer value.

**`doubleValue`** contains the returned double value.

**`boolValue`** contains the returned boolean value.

**`success`** used to determine the result of get operation.

**`type`** used to determine the type of returned value in string represent 
the types of value e.g. string, int, double, boolean, array, object, null and undefined.

**`typeNum`** used to determine the type of returned value in integer as represented by the following **`jsonDataType`** value.

**JSON_UNDEFINED = 0**

**JSON_OBJECT = 1**

**JSON_ARRAY = 2**

**JSON_STRING = 3**

**JSON_INT = 4**

**JSON_DOUBLE = 5**

**JSON_BOOL = 6 and**

**JSON_NULL = 7**



## License

The MIT License (MIT)

Copyright (c) 2019 K. Suwatchai (Mobizt)


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

