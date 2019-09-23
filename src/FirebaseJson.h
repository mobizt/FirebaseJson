
/*
 * FirebaseJson, version 2.0.0
 * 
 * The ESP8266/ESP32 Arduino library for create, edit and get the value from JSON object for Firebase.
 * 
 * September 22, 2019
 * 
 * 
 * This library was implement the zserge's JSON object parser library, jasmine JSMN which available here https://zserge.com/jsmn.html
 * 
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FirebaseJson_H
#define FirebaseJson_H

#include <Arduino.h>
#include "jsmn.h"
#include <memory>
#include <type_traits>

static const char FirebaseJson_STR_1[] PROGMEM = ",";
static const char FirebaseJson_STR_2[] PROGMEM = "\"";
static const char FirebaseJson_STR_3[] PROGMEM = ":";
static const char FirebaseJson_STR_4[] PROGMEM = "%d";
static const char FirebaseJson_STR_5[] PROGMEM = "%f";
static const char FirebaseJson_STR_6[] PROGMEM = "false";
static const char FirebaseJson_STR_7[] PROGMEM = "true";
static const char FirebaseJson_STR_8[] PROGMEM = "{";
static const char FirebaseJson_STR_9[] PROGMEM = "}";
static const char FirebaseJson_STR_10[] PROGMEM = "[";
static const char FirebaseJson_STR_11[] PROGMEM = "]";
static const char FirebaseJson_STR_12[] PROGMEM = "string";
static const char FirebaseJson_STR_13[] PROGMEM = "int";
static const char FirebaseJson_STR_14[] PROGMEM = "double";
static const char FirebaseJson_STR_15[] PROGMEM = "bool";
static const char FirebaseJson_STR_16[] PROGMEM = "object";
static const char FirebaseJson_STR_17[] PROGMEM = "array";
static const char FirebaseJson_STR_18[] PROGMEM = "null";
static const char FirebaseJson_STR_19[] PROGMEM = "undefined";
static const char FirebaseJson_STR_20[] PROGMEM = ".";
static const char FirebaseJson_STR_21[] PROGMEM = "\"root\":";
static const char FirebaseJson_STR_22[] PROGMEM = "    ";
static const char FirebaseJson_STR_23[] PROGMEM = ",";
static const char FirebaseJson_STR_24[] PROGMEM = "\n";
static const char FirebaseJson_STR_25[] PROGMEM = ": ";
static const char FirebaseJson_STR_26[] PROGMEM = "root";
static const char FirebaseJson_STR_27[] PROGMEM = "/";

class FirebaseJson;
class FirebaseJsonArray;

typedef enum
{
    JSON_UNDEFINED = 0,
    JSON_OBJECT = 1,
    JSON_ARRAY = 2,
    JSON_STRING = 3,
    JSON_INT = 4,
    JSON_DOUBLE = 5,
    JSON_BOOL = 6,
    JSON_NULL = 7
} jsonDataType;

typedef enum
{
    PRINT_MODE_NONE = -1,
    PRINT_MODE_PLAIN = 0,
    PRINT_MODE_PRETTY = 1
} printMode;

typedef enum
{
    COMP_MODE_REPLACE_OBJ = 0,
    COMP_MODE_REPLACE_ARR = 1,
    COMP_MODE_REMOVE_OBJ = 2,
    COMP_MODE_REMOVE_ARR = 3,
    COMP_MODE_INSERT_OBJ = 4,
    COMP_MODE_INSERT_ARR = 5
} compileMode;

class FirebaseJsonData
{
    friend class FirebaseJson;
    friend class FirebaseJsonArray;

public:
    FirebaseJsonData();
    ~FirebaseJsonData();


    /*
    Get array data as FirebaseJsonArray object from FirebaseJsonData object.
    
    @param jsonArray - The FirebaseJsonArray object to get.

    @return bool status for success operation.

    This should call after pares or get function.

   */
    bool getArray(FirebaseJsonArray &jsonArray);

    /*
    Get array data as FirebaseJson object from FirebaseJsonData object.
    
    @param jsonArray - The FirebaseJson object to get.

    @return bool status for success operation.

    This should call after pares or get function.

   */
    bool getJSON(FirebaseJson &json);

    String stringValue = "";
    int intValue = 0;
    double doubleValue = 0.0;
    bool boolValue = false;
    String type = "";
    uint8_t typeNum = 0;
    bool success = false;

private:
    int _type = 0;
    int _k_start = 0;
    int _start = 0;
    int _end = 0;
    int _o_index = -1;
    int _a_index = -1;
    int _tokenIndex = 0;
    int _depth = 0;
    int _len = 0;
    std::string _bufStr = "";
};

class FirebaseJson
{
    friend class FirebaseJsonArray;
    friend class FirebaseJsonData;

public:
    FirebaseJson();
    FirebaseJson(std::string &data);
    ~FirebaseJson();

    /*
    Clear buffer and tokens pointer of FirebaseJson object.
    
    @return instance of an object.

   */
    FirebaseJson &clear();

    /*
    Set JSON data (string) to FirebaseJson object.
    
    @param data - The JSON object string.

    @return instance of an object.

   */
    FirebaseJson &setJsonData(const String &data);

    /*
    Add null to FirebaseJson object.
    
    @param key - The new key string to add null.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key);

    /*
    Add string to FirebaseJson object.
    
    @param key - The new key string to add.

    @param value - The String value for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, const String &value);

    /*
    Add string to FirebaseJson object.
    
    @param key - The new key string to add.

    @param value - The char array for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, const char *value);

    /*
    Add integer to FirebaseJson object.
    
    @param key - The new key string to add.

    @param value - The integer value for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, int value);

    /*
    Add double to FirebaseJson object.
    
    @param key - The new key string to add.

    @param value - The double value for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, double value);

    /*
    Add boolean to FirebaseJson object.
    
    @param key - The new key string to add.

    @param value - The boolean value for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, bool value);

    /*
    Add nested FirebaseJson object to FirebaseJson object.
    
    @param key - The new key string to add.

    @param json - The FirebaseJson object for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, FirebaseJson &json);

    /*
    Add nested FirebaseJsonArray object to FirebaseJson object.
    
    @param key - The new key string to add.

    @param arr - The FirebaseJsonArray for new specified key.

    @return instance of an object.

   */
    FirebaseJson &add(const String &key, FirebaseJsonArray &arr);

    /*
    Get the FirebaseJson object serialized string.

    @param prettify - Boolean flag for return the pretty format string i.e. with text indent and newline. 

    @return serialized string of JSON object.

   */
    String toString(bool prettify = false);

    /*
    Get the value from specified node path in FirebaseJson object.

    @param jsonData - The FirebaseJsonData that hold the returned data.

    @param path - Relative path to the node in FirebaseJson object.  

    @return boolean status of operation.

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

   */
    bool get(FirebaseJsonData &jsonData, const String &path);

    /*
    Parse FirebaseJson object.

    @return instance of an object.

   */
    FirebaseJson &parse();

    /*
    Get the numbers of JSON tokens (keys + values + 1) to be available for read from jsonObjectIterator.  

    @return numbers of tokens.

   */
    size_t getJsonObjectIteratorCount();

    /*
    Read data from JSON objects by providing tokens ID.
    
    @param index - The referenced token index. This will auto increase to the next token index after read.

    @param key - The referenced key data string. This provided the key data output.

    @param value - The referenced value string. This provided the value of current key output.   

    @return instance of an object.

   */
    FirebaseJson &jsonObjectiterator(size_t &index, String &key, String &value);

    /*
    Get the parse result of JSON object.   

    @return FirebaseJsonData.

    FirebaseJsonData holds the return data as in get function above

   */
    FirebaseJsonData parseResult();

    /*
    Set null to FirebaseJson object at specified node path.
    
    @param path - The relative path to set null.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path);

    /*
    Set String value to FirebaseJson object at specified node path.
    
    @param path - The relative path to set value.

    @param value - The string value to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, const String &value);

    /*
    Set String value to FirebaseJson object at specified node path.
    
    @param path - The relative path to set value.

    @param value - The char array to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, const char *value);

    /*
    Set integer value to FirebaseJson object at specified node path.
    
    @param path - The relative path to set value.

    @param value - The integer value to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, int value);

    /*
    Set double value to FirebaseJson object at specified node path.
    
    @param path - The relative path to set value.

    @param value - The double value to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, double value);

    /*
    Set boolean value to FirebaseJson object at specified node path.
    
    @param path - The relative path to set value.

    @param value - The boolean value to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, bool value);

    /*
    Set nested FirebaseJson object to FirebaseJson object at specified node path.
    
    @param path - The relative path to set the nested FirebaseJson object.

    @param json - The FirebaseJson object to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, FirebaseJson &json);

    /*
    Set nested FirebaseJsonAtrray object to FirebaseJson object at specified node path.
    
    @param path - The relative path to set the nested FirebaseJsonAtrray object.

    @param arr - The FirebaseJsonAtrray object to set.

    @return bool value represents the success operation.

    The relative path can be mixed with array index (index number of array inside square brackets) and node names 
    e.g. /myRoot/[2]/Sensor1/myData/[3].

   */
    bool set(const String &path, FirebaseJsonArray &arr);

    /*
    Remove specified node and its content.

    @param path - The relative path to remove content and children.

    @return bool value represents the success operation.
    */
    bool remove(const String &path);




    template <typename T>
    FirebaseJson &add(const String &key, T value);
    template <typename T>
    bool set(const String &path, T value);

private:
    int _nextToken = 0;
    bool _tokenMatch = false;
    bool _skipNode = false;
    bool _insertNode = false;
    int _replaceToken = -1;
    int _insertToken = -1;
    int _pathNum = -1;
    int _pathIndex = -1;
    int _maxRound = 0;
    int _arrInsIndex = -1;
    int _compileAttempt = -1;
    bool _parseArray = false;
    int _nextDept = 0;
    int _parseDept = 0;
    std::string _jstr = "";
    std::vector<std::string> _pathTk = std::vector<std::string>();

    int _tokenCount = 0;
    bool _skipChild = false;
    bool _paresRes = false;
    FirebaseJsonData _jsonParseResult;
    std::string _lastKey = "";
    FirebaseJsonData _jsonObj;
    std::unique_ptr<jsmn_parser> _parser = std::unique_ptr<jsmn_parser>(new jsmn_parser());
    std::unique_ptr<jsmntok_t> _tokens = nullptr;

    FirebaseJson &setJsonData(std::string &data);
    FirebaseJson &_add(const char *key, const char *value, size_t klen, size_t vlen, bool isString = true, bool isJson = true);
    FirebaseJson &addArrayStr(const char *value, size_t len, bool isString);
    void resetParseResult();
    void setElementType();
    void _addString(const std::string &key, const std::string &value);
    void _addArray(const std::string &key, FirebaseJsonArray *arr);
    void _addInt(const std::string &key, int value);
    void _addDouble(const std::string &key, double value);
    void _addBool(const std::string &key, bool value);
    void _addNull(const std::string &key);
    void _addJson(const std::string &key, FirebaseJson *json);
    bool _setString(const std::string &path, const std::string &value);
    bool _setInt(const std::string &path, int value);
    bool _setDouble(const std::string &path, double value);
    bool _setBool(const std::string &path, bool value);
    bool _setNull(const std::string &path);
    bool _setJson(const std::string &path, FirebaseJson *json);
    bool _setArray(const std::string &path, FirebaseJsonArray *arr);
    bool _set(const char *path, int mode, const char *data);
    void _parse(const char *path);
    void clearPathTk();
    bool _compile(const char *path, int mode, const char *replace);
    void _parse();
    int parseToken(char *buf, int currentToken, int depth, const char *searchKey, int searchIndex, int ntype, int _printMode = -1);
    int compileToken(char *buf, int currentToken, int depth, int mode, const char *replace, int insertType = -1);
    void addReplacer(std::string &str, const char *replace);
    void addPathTk(std::string &str, int index);
    void addPlaceholder(std::string &str, int type);
    void addComma(std::string &str, const char *cm);
    void addBrk(std::string &str, const char *brk);
    bool isArrTk(int index);
    bool isStrTk(int index);
    int getArrIndex(int index);

    void _get(const char *key, int index = -1);
    std::string getString();
    void ltrim(std::string &str, const std::string &chars = " ");
    void rtrim(std::string &str, const std::string &chars = " ");
    void trim(std::string &str, const std::string &chars = " ");
    std::string toStdString(bool isJson = true);
    void strToTk(const std::string &str, std::vector<std::string> &cont, char delim);
    int strpos(const char *haystack, const char *needle, int offset);
    int rstrpos(const char *haystack, const char *needle, int offset);
    char *rstrstr(const char *haystack, const char *needle);
};

class FirebaseJsonArray
{

public:
    FirebaseJsonArray();
    ~FirebaseJsonArray();

    friend class FirebaseJson;
    friend class FirebaseJsonData;

    template <typename T>
    FirebaseJsonArray &add(T value);
    FirebaseJsonArray &add();
    FirebaseJsonArray &add(const String &value);
    FirebaseJsonArray &add(const char *value);
    FirebaseJsonArray &add(int value);
    FirebaseJsonArray &add(double value);
    FirebaseJsonArray &add(bool value);
    FirebaseJsonArray &add(FirebaseJson &json);
    FirebaseJsonArray &add(FirebaseJsonArray &arr);

    /*
    Get the array value at specified index from FirebaseJsonArray object.

    @param jsonObj - FirebaseJsonData object that holds data at specified index returned FirebaseJsonArray object.

    @param index - Index of data in FirebaseJsonArray object.    

    @return boolean status of operation.

   */
    bool get(FirebaseJsonData &jsonObj, int index);



    /*
    Get the array value at specified path from FirebaseJsonArray object.

    @param jsonObj - FirebaseJsonData object that holds data at specified path returned FirebaseJsonArray object.

    @param path - Relative path to data in FirebaseJsonArray object.    

    @return boolean status of operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2

   */
    bool get(FirebaseJsonData &jsonObj, const String &path);

    /*
    Get the length of array in FirebaseJsonArray object.  

    @return length of array.

   */
    size_t size();

    /*
    Get the FirebaseJsonArray object serialized string.

    @param prettify - Boolean flag for return the pretty format string i.e. with text indent and newline. 

    @return serialized string of JSON Array object.

   */
    String toString(bool prettify = false);

    /*
    Clear array in FirebaseJsonArray object.

    @return instance of an object.

   */
    FirebaseJsonArray &clear();

    /*
    Set null to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set null.

    @return bool value represents the success operation.

   */
    bool set(int index);

    /*
    Set String to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set value.

    @param value - The String to set.

    @return bool value represents the success operation.

   */
    bool set(int index, const String &value);

    /*
    Set String to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set value.

    @param value - The char array to set.

    @return bool value represents the success operation.

   */
    bool set(int index, const char *value);

    /*
    Set integer value to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set value.

    @param value - The integer value to set.

    @return bool value represents the success operation.

   */
    bool set(int index, int value);

    /*
    Set double value to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set value.

    @param value - The double value to set.

    @return bool value represents the success operation.

   */
    bool set(int index, double value);

    /*
    Set boolean value to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set value.

    @param value - The boolean value to set.

    @return bool value represents the success operation.

   */
    bool set(int index, bool value);

    /*
    Set nested FirebaseJson object to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set nested FirebaseJson object.

    @param value - The FirebaseJson object to set.

    @return bool value represents the success operation.

   */
    bool set(int index, FirebaseJson &json);

    /*
    Set nested FirebaseJsonArray object to FirebaseJsonArray object at specified index.
    
    @param index - The array index to set nested FirebaseJsonArray object.

    @param value - The FirebaseJsonArray object to set.

    @return bool value represents the success operation.

   */
    bool set(int index, FirebaseJsonArray &arr);

    
    /*
    Set null to FirebaseJson object at specified path.
    
    @param path - The relative path to set null.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path);

    /*
    Set String to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The String to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, const String &value);

    /*
    Set string to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The char array to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, const char *value);

    /*
    Set integer value to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The integer value to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, int value);

    /*
    Set double value to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The double to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, double value);

    /*
    Set boolean value to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The boolean value to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, bool value);

    /*
    Set the nested FirebaseJson object to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The FirebaseJson object to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, FirebaseJson &json);

    /*
    Set the nested FirebaseJsonArray object to FirebaseJsonArray object at specified path.
    
    @param path - The relative path to set value.

    @param value - The FirebaseJsonArray object to set.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would get the data from myData key inside the array indexes 2.

   */
    bool set(const String &path, FirebaseJsonArray &arr);

    /*
    Remove the array value at specified index from FirebaseJsonArray object.

    @param index - The array index to remove.

    @return bool value represents the success operation.

    */
    bool remove(int index);

    /*
    Remove the array value at specified path from FirebaseJsonArray object.

    @param path - The relative path to array in FirebaseJsonArray object to remove.

    @return bool value represents the success operation.

    The relative path must be begin with array index (index number of array inside square brackets) followed by 
    other array indexes or node names e.g. /[2]/myData would remove the data of myData key inside the array indexes 2.
    
    */
    bool remove(const String &path);




    template <typename T>
    bool set(int index, T value);
    template <typename T>
    bool set(const String &path, T value);

private:
    std::string _jstr = "";
    FirebaseJson _json;
    size_t _arrLen = 0;

    void _addString(const std::string &value);
    void _addInt(int value);
    void _addDouble(double value);
    void _addBool(bool value);
    void _addNull();
    void _addJson(FirebaseJson *json);
    void _addArray(FirebaseJsonArray *arr);
    bool _setString(int index, const std::string &value);
    bool _setString(const String &path, const std::string &value);
    bool _setInt(int index, int value);
    bool _setInt(const String &path, int value);
    bool _setDouble(int index, double value);
    bool _setDouble(const String &path, double value);
    bool _setBool(int index, bool value);
    bool _setBool(const String &path, bool value);
    bool _setNull(int index);
    bool _setNull(const String &path);
    bool _setJson(int index, FirebaseJson *json);
    bool _setJson(const String &path, FirebaseJson *json);
    bool _setArray(int index, FirebaseJsonArray *arr);
    bool _setArray(const String &path, FirebaseJsonArray *arr);
    std::string toStdString();
    void _set2(int index, const char *value, bool isStr = true);
    void _set(const char *path, const char *value, bool isStr = true);
    bool _get(FirebaseJsonData &jsonObj, const char *path);
    bool _remove(const char *path);
    void setEmptyArr(std::string &str);
};

#endif