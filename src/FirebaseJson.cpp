
/*
 * FirebaseJson, version 2.0.0
 * 
 * The ESP8266/ESP32 Arduino library for create, edit and get the value from JSON object for Firebase.
 * 
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

#include "FirebaseJson.h"

FirebaseJson::FirebaseJson(){};

FirebaseJson::FirebaseJson(std::string &data)
{
  setJsonData(data);
}

FirebaseJson::~FirebaseJson()
{
  clear();
  _parser.reset();
  _parser.release();
  _parser = nullptr;
}

FirebaseJson &FirebaseJson::setJsonData(std::string &data)
{
  return setJsonData(data.c_str());
}

FirebaseJson &FirebaseJson::setJsonData(const String &data)
{
  char *tmp = new char[10];
  memset(tmp, 0, 10);
  strcpy_P(tmp, FirebaseJson_STR_8);
  int p1 = strpos(data.c_str(), tmp, 0);
  memset(tmp, 0, 10);
  strcpy_P(tmp, FirebaseJson_STR_9);
  int p2 = rstrpos(data.c_str(), tmp, data.length() - 1);
  if (p1 != -1)
    p1 += 1;
  if (p1 != -1 && p2 != -1)
    _jstr = data.substring(p1, p2).c_str();
  delete[] tmp;
  return *this;
}

FirebaseJson &FirebaseJson::clear()
{
  std::string().swap(_jstr);
  std::string().swap(_lastKey);
  _tokens.reset();
  _tokens.release();
  _tokens = nullptr;
  return *this;
}

FirebaseJson &FirebaseJson::add(const String &key)
{
  _addNull(key.c_str());
  return *this;
}

FirebaseJson &FirebaseJson::add(const String &key, const String &value)
{
  _addString(key.c_str(), value.c_str());
  return *this;
}
FirebaseJson &FirebaseJson::add(const String &key, const char *value)
{
  _addString(key.c_str(), value);
  return *this;
}
FirebaseJson &FirebaseJson::add(const String &key, int value)
{
  _addInt(key.c_str(), value);
  return *this;
}

FirebaseJson &FirebaseJson::add(const String &key, double value)
{
  _addDouble(key.c_str(), value);
  return *this;
}

FirebaseJson &FirebaseJson::add(const String &key, bool value)
{
  _addBool(key.c_str(), value);
  return *this;
}

FirebaseJson &FirebaseJson::add(const String &key, FirebaseJson &json)
{
  _addJson(key.c_str(), &json);
  return *this;
}
FirebaseJson &FirebaseJson::add(const String &key, FirebaseJsonArray &arr)
{
  _addArray(key.c_str(), &arr);
  return *this;
}

template <typename T>
FirebaseJson &FirebaseJson::add(const String &key, T value)
{
  if (std::is_same<T, int>::value)
    _addInt(key, value);
  else if (std::is_same<T, double>::value)
    _addDouble(key, value);
  else if (std::is_same<T, bool>::value)
    _addBool(key, value);
  else if (std::is_same<T, const char *>::value)
    _addString(key, value);
  else if (std::is_same<T, FirebaseJson &>::value)
    _addJson(key, &value);
  else if (std::is_same<T, FirebaseJsonArray &>::value)
    _addArray(key, &value);
  return *this;
}

void FirebaseJson::_addString(const std::string &key, const std::string &value)
{
  _add(key.c_str(), value.c_str(), key.length(), value.length(), true, true);
}

void FirebaseJson::_addInt(const std::string &key, int value)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_4);
  sprintf(buf, b, value);
  _add(key.c_str(), buf, key.length(), 60, false, true);
  delete[] b;
  delete[] buf;
}

void FirebaseJson::_addDouble(const std::string &key, double value)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_5);
  sprintf(buf, b, value);
  _add(key.c_str(), buf, key.length(), 60, false, true);
  delete[] b;
  delete[] buf;
}

void FirebaseJson::_addBool(const std::string &key, bool value)
{
  size_t bufSize = 20;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  if (value)
    strcat_P(buf, FirebaseJson_STR_7);
  else
    strcat_P(buf, FirebaseJson_STR_6);
  _add(key.c_str(), buf, key.length(), bufSize, false, true);
  delete[] buf;
}

void FirebaseJson::_addNull(const std::string &key)
{
  size_t bufSize = 20;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  strcat_P(buf, FirebaseJson_STR_18);
  _add(key.c_str(), buf, key.length(), bufSize, false, true);
  delete[] buf;
}

void FirebaseJson::_addJson(const std::string &key, FirebaseJson *json)
{
  _add(key.c_str(), json->toStdString().c_str(), key.length(), json->toStdString().length(), false, true);
}

void FirebaseJson::_addArray(const std::string &key, FirebaseJsonArray *arr)
{
  _add(key.c_str(), arr->toString().c_str(), key.length(), arr->toString().length(), false, true);
}

String FirebaseJson::toString(bool prettify)
{
  _parse();
  char *et = new char[2];
  memset(et, 0, 2);
  if (_paresRes)
  {
    _tokenMatch = false;
    int bufLen = toStdString().length() + 1;
    char *buf = new char[bufLen];
    memset(buf, 0, bufLen);
    strcpy(buf, toStdString().c_str());
    int mode = prettify ? PRINT_MODE_PRETTY : PRINT_MODE_PLAIN;
    if (_jsonObj.success)
      parseToken(buf, _nextToken, _nextDept, et, -1, -1, mode);
    delete[] buf;
    _tokens.reset();
    _tokens.release();
    _tokens = nullptr;
    delete[] et;
    return _jsonObj._bufStr.c_str();
  }
  _tokens.reset();
  _tokens.release();
  _tokens = nullptr;
  delete[] et;
  return std::string().c_str();
}

std::string FirebaseJson::toStdString(bool isJson)
{
  std::string ret = "";
  size_t bufSize = 10;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  if (isJson)
    strcat_P(buf, FirebaseJson_STR_8);
  else
    strcat_P(buf, FirebaseJson_STR_10);
  ret += buf;
  ret += _jstr;
  memset(buf, 0, bufSize);
  if (isJson)
    strcat_P(buf, FirebaseJson_STR_9);
  else
    strcat_P(buf, FirebaseJson_STR_11);
  ret += buf;
  delete[] buf;
  return ret;
}

FirebaseJson &FirebaseJson::_add(const char *key, const char *value, size_t klen, size_t vlen, bool isString, bool isJson)
{
  size_t bufSize = klen + vlen + 10;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  if (_jstr.length() > 0)
    strcpy_P(buf, FirebaseJson_STR_1);
  if (isJson)
  {
    strcat_P(buf, FirebaseJson_STR_2);
    strcat(buf, key);
    strcat_P(buf, FirebaseJson_STR_2);
    strcat_P(buf, FirebaseJson_STR_3);
  }
  if (isString)
    strcat_P(buf, FirebaseJson_STR_2);
  strcat(buf, value);
  if (isString)
    strcat_P(buf, FirebaseJson_STR_2);
  _jstr += buf;
  delete[] buf;
  return *this;
}

FirebaseJson &FirebaseJson::addArrayStr(const char *value, size_t len, bool isString)
{
  char *et = new char[2];
  memset(et, 0, 2);
  _add(et, value, 0, len, isString, false);
  delete[] et;
  return *this;
}

bool FirebaseJson::get(FirebaseJsonData &jsonData, const String &path)
{
  clearPathTk();
  strToTk(path.c_str(), _pathTk, '/');
  _parse(path.c_str());
  if (_jsonObj.success)
  {
    if (_jsonObj._type == JSMN_STRING && _jsonObj.stringValue[0] == '"' && _jsonObj.stringValue[_jsonObj.stringValue.length() - 1] == '"')
      _jsonObj.stringValue = _jsonObj.stringValue.substring(1, _jsonObj.stringValue.length() - 1);
    jsonData = _jsonObj;
  }
  clearPathTk();
  _tokens.reset();
  _tokens.release();
  _tokens = nullptr;
  return _jsonObj.success;
}

FirebaseJson &FirebaseJson::parse()
{
  _parse();
  return *this;
}

void FirebaseJson::_parse()
{
  int bufLen = toStdString().length() + 1;
  char *buf = new char[bufLen];
  memset(buf, 0, bufLen);
  strcpy(buf, toStdString().c_str());
  _tokens.reset();
  _tokens.release();
  int cnt = jsmn_parse(_parser.get(), buf, bufLen, (jsmntok_t *)NULL, 0) + 150;
  _tokens = std::unique_ptr<jsmntok_t>(new jsmntok_t[cnt + 1]);
  jsmn_init(_parser.get());
  _tokenCount = jsmn_parse(_parser.get(), buf, bufLen, _tokens.get(), cnt + 1);
  _paresRes = true;
  if (_tokenCount < 0)
    _paresRes = false;
  if (_tokenCount < 1 || _tokens.get()[0].type != JSMN_OBJECT)
    _paresRes = false;
  _jsonObj.success = _paresRes;
  _nextToken = 0;
  _nextDept = 0;
  _tokenMatch = false;
  _replaceToken = -1;
  _insertToken = -1;
  resetParseResult();
  setElementType();
  delete[] buf;
}

int FirebaseJson::parseToken(char *buf, int i, int depth, const char *searchKey, int searchIndex, int ntype, int printMode)
{
  if (_tokenMatch)
    return i;
  char *tab = new char[6];
  char *brk1 = new char[3];
  char *brk2 = new char[3];
  char *brk3 = new char[3];
  char *brk4 = new char[3];
  char *cm = new char[3];
  char *nl = new char[3];
  char *pr = new char[4];
  char *pr2 = new char[3];
  memset(brk1, 0, 3);
  memset(brk2, 0, 3);
  memset(brk3, 0, 3);
  memset(brk4, 0, 3);
  memset(cm, 0, 3);
  memset(pr2, 0, 3);
  strcpy_P(brk1, FirebaseJson_STR_8);
  strcpy_P(brk2, FirebaseJson_STR_9);
  strcpy_P(brk3, FirebaseJson_STR_10);
  strcpy_P(brk4, FirebaseJson_STR_11);
  strcpy_P(cm, FirebaseJson_STR_23);
  strcpy_P(pr2, FirebaseJson_STR_3);
  if (printMode == PRINT_MODE_PRETTY)
  {
    memset(tab, 0, 6);
    memset(nl, 0, 3);
    memset(pr, 0, 4);
    strcpy_P(tab, FirebaseJson_STR_22);
    strcpy_P(nl, FirebaseJson_STR_24);
    strcpy_P(pr, FirebaseJson_STR_25);
  }
  jsmntok_t *h = &_tokens.get()[i];
  if (h->type == JSMN_OBJECT)
  {
    if (printMode != PRINT_MODE_NONE)
    {
      if (ntype > -1 && printMode == PRINT_MODE_PRETTY)
      {
        for (int j = 0; j < depth; j++)
          _jsonObj._bufStr += tab;
      }
      _jsonObj._bufStr += brk1;
      if (printMode == PRINT_MODE_PRETTY && h->size > 0)
        _jsonObj._bufStr += nl;
    }
    i++;
    int len = h->size;
    for (int n = 0; n < len; n++)
    {
      h = &_tokens.get()[i];
      size_t len1 = h->end - h->start + 3;
      char *k = new char[len1];
      memset(k, 0, len1);
      strncpy(k, buf + h->start, h->end - h->start);
      if (printMode != PRINT_MODE_NONE)
      {
        size_t len1 = h->end - h->start + 3;
        char *k2 = new char[len1];
        memset(k2, 0, len1);
        if (buf[h->start - 1] != '"')
          strncpy(k2, buf + h->start, h->end - h->start);
        else
          strncpy(k2, buf + h->start - 1, h->end - h->start + 2);
        if (printMode == PRINT_MODE_PRETTY && h->size > 0)
        {
          for (int j = 0; j < depth; j++)
            _jsonObj._bufStr += tab;
          _jsonObj._bufStr += tab;
        }
        _jsonObj._bufStr += k2;
        if (printMode == PRINT_MODE_PRETTY)
          _jsonObj._bufStr += pr;
        else
          _jsonObj._bufStr += pr2;
        delete[] k2;
      }
      if (strcmp(searchKey, k) == 0)
      {
        jsmntok_t *g = &_tokens.get()[i + 1];
        size_t len2 = g->end - g->start + 3;
        char *v = new char[len2];
        memset(v, 0, len2);
        strncpy(v, buf + g->start, g->end - g->start);
        _nextDept++;
        _nextToken = i + 1;
        _replaceToken = i + 1;
        _insertToken = i + 1;
        _tokenMatch = true;
        _jsonObj.stringValue = v;
        _jsonObj._type = g->type;
        _jsonObj._o_index = n;
        _jsonObj._k_start = h->start;
        _jsonObj._start = g->start;
        _jsonObj._end = g->end;
        _jsonObj._tokenIndex = i;
        _jsonObj._depth = depth;
        _jsonObj._len = len;
        _jsonObj.success = true;
        setElementType();
        delete[] k;
        delete[] v;
        break;
      }
      delete[] k;
      if (_tokenMatch)
        break;
      i++;
      h = &_tokens.get()[i];
      if (h->type == JSMN_OBJECT)
      {
        i = parseToken(buf, i, depth + 1, searchKey, searchIndex, -1, printMode);
        if (printMode != PRINT_MODE_NONE)
        {
          if (n < len - 1)
            _jsonObj._bufStr += cm;
          if (printMode == PRINT_MODE_PRETTY)
            _jsonObj._bufStr += nl;
        }
      }
      else if (h->type == JSMN_ARRAY)
      {
        i = parseToken(buf, i, depth + 1, searchKey, searchIndex, -1, printMode);
        if (printMode != PRINT_MODE_NONE)
        {
          if (n < len - 1)
            _jsonObj._bufStr += cm;
          if (printMode == PRINT_MODE_PRETTY)
            _jsonObj._bufStr += nl;
        }
      }
      else
      {
        if (printMode != PRINT_MODE_NONE)
        {
          size_t len2 = h->end - h->start + 3;
          char *v = new char[len2];
          memset(v, 0, len2);
          if (buf[h->start - 1] != '"')
            strncpy(v, buf + h->start, h->end - h->start);
          else
            strncpy(v, buf + h->start - 1, h->end - h->start + 2);
          _jsonObj._bufStr += v;
          if (printMode != PRINT_MODE_NONE)
          {
            if (n < len - 1)
              _jsonObj._bufStr += cm;
            if (printMode == PRINT_MODE_PRETTY)
              _jsonObj._bufStr += nl;
          }
          delete[] v;
        }
        i++;
      }
    }
    if (printMode != PRINT_MODE_NONE)
    {
      if (printMode == PRINT_MODE_PRETTY && len > 0)
      {
        for (int j = 0; j < depth; j++)
          _jsonObj._bufStr += tab;
      }
      _jsonObj._bufStr += brk2;
    }
  }
  else if (h->type == JSMN_ARRAY)
  {
    i++;
    int len = h->size;
    if (printMode != PRINT_MODE_NONE)
    {
      if (ntype > -1 && printMode == PRINT_MODE_PRETTY)
      {
        for (int j = 0; j < depth; j++)
          _jsonObj._bufStr += tab;
      }
      _jsonObj._bufStr += brk3;
      if (printMode == PRINT_MODE_PRETTY && h->size > 0)
        _jsonObj._bufStr += nl;
    }
    for (int n = 0; n < len; n++)
    {
      h = &_tokens.get()[i];
      if (searchIndex == n && depth == _nextDept)
      {
        if (h->type == JSMN_OBJECT || h->type == JSMN_ARRAY)
          _nextToken = i;
        else
          _nextToken = i + 1;
        _replaceToken = i;
        _insertToken = i;
        _tokenMatch = true;
        char *v = new char[h->end - h->start + 3];
        memset(v, 0, h->end - h->start + 3);
        if (buf[h->start - 1] != '"')
          strncpy(v, buf + h->start, h->end - h->start);
        else
          strncpy(v, buf + h->start - 1, h->end - h->start + 2);
        _nextDept++;
        _jsonObj.stringValue = v;
        _jsonObj._type = h->type;
        _jsonObj._k_start = 0;
        _jsonObj._a_index = n;
        _jsonObj._start = h->start;
        _jsonObj._end = h->end;
        _jsonObj._tokenIndex = i;
        _jsonObj._depth = depth;
        _jsonObj._len = len;
        _jsonObj.success = true;
        setElementType();
        delete[] v;
        break;
      }
      if (_tokenMatch)
        break;
      if (h->type == JSMN_OBJECT)
      {
        i = parseToken(buf, i, depth + 1, searchKey, searchIndex, 0, printMode);
        if (printMode != PRINT_MODE_NONE)
        {
          if (n < len - 1)
            _jsonObj._bufStr += cm;
          if (printMode == PRINT_MODE_PRETTY)
            _jsonObj._bufStr += nl;
        }
      }
      else if (h->type == JSMN_ARRAY)
      {
        i = parseToken(buf, i, depth + 1, searchKey, searchIndex, 0, printMode);
        if (printMode != PRINT_MODE_NONE)
        {
          if (n < len - 1)
            _jsonObj._bufStr += cm;
          if (printMode == PRINT_MODE_PRETTY)
            _jsonObj._bufStr += nl;
        }
      }
      else
      {
        if (printMode != PRINT_MODE_NONE)
        {
          char *v = new char[h->end - h->start + 3];
          memset(v, 0, h->end - h->start + 3);
          if (buf[h->start - 1] != '"')
            strncpy(v, buf + h->start, h->end - h->start);
          else
            strncpy(v, buf + h->start - 1, h->end - h->start + 2);
          if (printMode == PRINT_MODE_PRETTY)
          {
            for (int j = 0; j < depth; j++)
              _jsonObj._bufStr += tab;
            _jsonObj._bufStr += tab;
          }
          _jsonObj._bufStr += v;
          if (printMode != PRINT_MODE_NONE)
          {
            if (n < len - 1)
              _jsonObj._bufStr += cm;
            if (printMode == PRINT_MODE_PRETTY)
              _jsonObj._bufStr += nl;
          }
          delete[] v;
        }
        i++;
      }
    }
    if (printMode != PRINT_MODE_NONE)
    {
      if (printMode == PRINT_MODE_PRETTY && len > 0)
      {
        for (int j = 0; j < depth; j++)
          _jsonObj._bufStr += tab;
      }
      _jsonObj._bufStr += brk4;
    }
  }
  delete[] tab;
  delete[] brk1;
  delete[] brk2;
  delete[] brk3;
  delete[] brk4;
  delete[] cm;
  delete[] nl;
  delete[] pr;
  delete[] pr2;
  return i;
}

int FirebaseJson::compileToken(char *buf, int i, int depth, int mode, const char *replace, int insertType)
{
  char *brk1 = new char[3];
  char *brk2 = new char[3];
  char *brk3 = new char[3];
  char *brk4 = new char[3];
  char *cm = new char[3];
  char *pr = new char[3];
  char *qm = new char[3];
  memset(brk1, 0, 3);
  memset(brk2, 0, 3);
  memset(brk3, 0, 3);
  memset(brk4, 0, 3);
  memset(cm, 0, 3);
  memset(pr, 0, 3);
  memset(qm, 0, 3);
  strcpy_P(brk1, FirebaseJson_STR_8);
  strcpy_P(brk2, FirebaseJson_STR_9);
  strcpy_P(brk3, FirebaseJson_STR_10);
  strcpy_P(brk4, FirebaseJson_STR_11);
  strcpy_P(cm, FirebaseJson_STR_23);
  strcpy_P(pr, FirebaseJson_STR_3);
  strcpy_P(qm, FirebaseJson_STR_2);
  bool skip1 = false;
  bool skip2 = false;
  bool skip3 = false;
  bool skip4 = false;
  int ins2 = -1;
  int ins3 = -1;
  int ins4 = -1;
  int type = 0;
  bool replaced = false;
  jsmntok_t *h = &_tokens.get()[i];
  if (!_skipNode && i == _replaceToken)
  {
    _skipNode = true;
    skip1 = true;
    if (mode == COMP_MODE_REPLACE_OBJ || COMP_MODE_REPLACE_ARR)
    {
      addReplacer(_jsonObj._bufStr, replace);
      replaced = true;
    }
  }
  if (h->type == JSMN_OBJECT)
  {
    if (!_skipNode)
      _jsonObj._bufStr += brk1;
    i++;
    int len = h->size;
    for (int n = 0; n < len; n++)
    {
      h = &_tokens.get()[i];
      if (!_skipNode && i == _replaceToken)
      {
        _skipNode = true;
        skip2 = true;
        if (mode == COMP_MODE_REPLACE_OBJ || COMP_MODE_REPLACE_ARR)
        {
          addReplacer(_jsonObj._bufStr, replace);
          replaced = true;
        }
      }
      if (!_skipNode)
      {
        size_t len1 = h->end - h->start + 3;
        char *k = new char[len1];
        memset(k, 0, len1);
        if (buf[h->start - 1] != '"')
          strncpy(k, buf + h->start, h->end - h->start);
        else
          strncpy(k, buf + h->start - 1, h->end - h->start + 2);
        addReplacer(_jsonObj._bufStr, k);
        addReplacer(_jsonObj._bufStr, pr);
        delete[] k;
      }
      i++;
      h = &_tokens.get()[i];
      if (!_skipNode && i == _replaceToken && !replaced)
      {
        _skipNode = true;
        skip4 = true;
        if (mode == COMP_MODE_REPLACE_OBJ || COMP_MODE_REPLACE_ARR)
        {
          addReplacer(_jsonObj._bufStr, replace);
          replaced = true;
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
        }
      }
      if (!_insertNode && i == _insertToken && mode != COMP_MODE_REMOVE_OBJ && mode != COMP_MODE_REMOVE_ARR)
      {
        _insertNode = true;
        ins4 = 0;
        type = h->type;
      }
      int insType = -1;
      if (ins4 == 0 && type == JSMN_OBJECT)
        insType = 0;
      else if (ins4 == 0 && type == JSMN_ARRAY)
        insType = 1;
      if (h->type == JSMN_OBJECT)
      {
        i = compileToken(buf, i, depth + 1, mode, replace, insType);
        if (!_skipNode)
        {
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
        }
      }
      else if (h->type == JSMN_ARRAY)
      {
        i = compileToken(buf, i, depth + 1, mode, replace, insType);
        if (!_skipNode)
        {
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
        }
      }
      else
      {
        if (!_skipNode)
        {
          size_t len2 = h->end - h->start + 3;
          char *v = new char[len2];
          memset(v, 0, len2);
          if (buf[h->start - 1] != '"')
            strncpy(v, buf + h->start, h->end - h->start);
          else
            strncpy(v, buf + h->start - 1, h->end - h->start + 2);
          if ((ins4 == 0 && n == _arrInsIndex) || (ins4 == 0 && _arrInsIndex == -1))
          {
            ins4 = -1;
            replaced = true;
            if (_pathIndex == _pathNum)
              addReplacer(_jsonObj._bufStr, replace);
            else
              addPlaceholder(_jsonObj._bufStr, 0);
          }
          else
          {
            if (_tokenMatch && strlen(replace) > 0)
            {
              if (mode == COMP_MODE_REPLACE_OBJ || COMP_MODE_REPLACE_ARR)
              {
                addReplacer(_jsonObj._bufStr, replace);
                replaced = true;
              }
            }
            else
              addReplacer(_jsonObj._bufStr, v);
          }
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
          delete[] v;
        }
        i++;
      }
      if (skip4)
        _skipNode = false;
      if (ins4 == 0)
      {
        ins4 = -1;
        _insertNode = false;
      }
      if (skip2)
        _skipNode = false;
      if (ins2 == 0)
      {
        ins2 = -1;
        _insertNode = false;
      }
    }
    if (insertType > -1 && !replaced && _pathIndex < _pathNum)
    {
      if (len > 0)
        addComma(_jsonObj._bufStr, cm);
      if (insertType == 0)
      {
        if (_arrInsIndex == -1)
        {
          addPlaceholder(_jsonObj._bufStr, 2);
          addPathTk(_jsonObj._bufStr, _pathIndex);
          addPlaceholder(_jsonObj._bufStr, 3);
          if (_pathIndex < _pathNum - 1)
          {
            if (isArrTk(_pathIndex + 1))
              addPlaceholder(_jsonObj._bufStr, 1);
            else
              addPlaceholder(_jsonObj._bufStr, 0);
          }
          else
            addReplacer(_jsonObj._bufStr, replace);
        }
      }
      else if (insertType == 1)
      {
        if (_arrInsIndex == -1)
        {
          addPlaceholder(_jsonObj._bufStr, 2);
          addPathTk(_jsonObj._bufStr, _pathIndex);
          addPlaceholder(_jsonObj._bufStr, 3);
          if (_pathIndex < _pathNum - 1)
          {
            if (isArrTk(_pathIndex + 1))
              addPlaceholder(_jsonObj._bufStr, 1);
            else
              addPlaceholder(_jsonObj._bufStr, 0);
          }
          else
            addReplacer(_jsonObj._bufStr, replace);
        }
        else
          addPlaceholder(_jsonObj._bufStr, 1);
      }
      replaced = true;
    }
    if (!_skipNode)
      addBrk(_jsonObj._bufStr, brk2);
  }
  else if (h->type == JSMN_ARRAY)
  {
    i++;
    int len = h->size;
    if (!_skipNode)
      addBrk(_jsonObj._bufStr, brk3);
    for (int n = 0; n < len; n++)
    {
      h = &_tokens.get()[i];
      if (!_insertNode && i == _insertToken && mode != COMP_MODE_REMOVE_OBJ && mode != COMP_MODE_REMOVE_ARR)
      {
        _insertNode = true;
        ins3 = 0;
        type = h->type;
      }
      if (!_skipNode && i == _replaceToken && !_insertNode)
      {
        _skipNode = true;
        skip3 = true;
        if (mode == COMP_MODE_REPLACE_OBJ || COMP_MODE_REPLACE_ARR)
        {
          addReplacer(_jsonObj._bufStr, replace);
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
        }
      }
      int insType = -1;
      if (ins3 == 0 && type == JSMN_OBJECT)
        insType = 1;
      else if (ins3 == 0 && type == JSMN_ARRAY)
        insType = 1;
      if (h->type == JSMN_OBJECT)
      {
        i = compileToken(buf, i, depth + 1, mode, replace, insType);
        if (!_skipNode)
        {
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
        }
      }
      else if (h->type == JSMN_ARRAY)
      {
        i = compileToken(buf, i, depth + 1, mode, replace, insType);
        if (!_skipNode)
        {
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
        }
      }
      else
      {
        if (!_skipNode)
        {
          char *v = new char[h->end - h->start + 3];
          memset(v, 0, h->end - h->start + 3);
          if (buf[h->start - 1] != '"')
            strncpy(v, buf + h->start, h->end - h->start);
          else
            strncpy(v, buf + h->start - 1, h->end - h->start + 2);
          if (ins3 == 0 && n == _arrInsIndex)
          {
            ins3 = -1;
            replaced = true;
            if (_pathIndex == _pathNum)
              addReplacer(_jsonObj._bufStr, replace);
            else
              addPlaceholder(_jsonObj._bufStr, 0);
          }
          else
          {
            if (_tokenMatch && strlen(replace) > 0)
            {
              if (mode == COMP_MODE_REPLACE_OBJ || mode == COMP_MODE_REPLACE_ARR)
                addReplacer(_jsonObj._bufStr, replace);
            }
            else
              addReplacer(_jsonObj._bufStr, v);
          }
          if (n < len - 1)
            addComma(_jsonObj._bufStr, cm);
          delete[] v;
        }
        i++;
      }
      if (skip3)
        _skipNode = false;
      if (ins3 == 0)
        ins3 = -1;
    }
    if (insertType > -1 && !replaced)
    {
      if (_arrInsIndex > -1 && insertType == 1)
      {
        if (len > 0)
          addComma(_jsonObj._bufStr, cm);
        if (!isArrTk(_pathIndex + 1))
          addPlaceholder(_jsonObj._bufStr, 0);
        else
          addPlaceholder(_jsonObj._bufStr, 1);
      }
      else if (_arrInsIndex == -1 && insertType == 1)
      {
        if (_pathIndex < _pathNum - 1)
        {
          addPlaceholder(_jsonObj._bufStr, 4);
          addPlaceholder(_jsonObj._bufStr, 2);
          addPathTk(_jsonObj._bufStr, _pathIndex);
          addPlaceholder(_jsonObj._bufStr, 3);
          if (isArrTk(_pathIndex + 1))
            addPlaceholder(_jsonObj._bufStr, 1);
          else
            addPlaceholder(_jsonObj._bufStr, 0);
          addPlaceholder(_jsonObj._bufStr, 5);
        }
        else
        {
          if (_pathIndex < _pathNum)
          {
            addPlaceholder(_jsonObj._bufStr, 4);
            addPlaceholder(_jsonObj._bufStr, 2);
            addPathTk(_jsonObj._bufStr, _pathIndex);
            addPlaceholder(_jsonObj._bufStr, 3);
            addReplacer(_jsonObj._bufStr, replace);
            addPlaceholder(_jsonObj._bufStr, 5);
          }
        }
      }
      replaced = true;
    }
    if (!_skipNode)
      addBrk(_jsonObj._bufStr, brk4);
  }
  if (skip1)
    _skipNode = false;
  delete[] brk1;
  delete[] brk2;
  delete[] brk3;
  delete[] brk4;
  delete[] cm;
  delete[] pr;
  delete[] qm;
  return i;
}

void FirebaseJson::addReplacer(std::string &str, const char *replace)
{
  str += replace;
}

void FirebaseJson::addPathTk(std::string &str, int index)
{
  str += _pathTk[index];
}

void FirebaseJson::addPlaceholder(std::string &str, int type)
{
  char *p = new char[6];
  memset(p, 0, 3);
  if (type == 0)
  {
    strcpy_P(p, FirebaseJson_STR_8);
    strcat_P(p, FirebaseJson_STR_9);
  }
  else if (type == 1)
  {
    strcpy_P(p, FirebaseJson_STR_10);
    strcat_P(p, FirebaseJson_STR_11);
  }
  else if (type == 2)
  {
    strcpy_P(p, FirebaseJson_STR_2);
  }
  else if (type == 3)
  {
    strcpy_P(p, FirebaseJson_STR_2);
    strcat_P(p, FirebaseJson_STR_3);
  }
  else if (type == 4)
  {
    strcpy_P(p, FirebaseJson_STR_8);
  }
  else if (type == 5)
  {
    strcpy_P(p, FirebaseJson_STR_9);
  }
  str += p;
  delete[] p;
}

void FirebaseJson::addComma(std::string &str, const char *cm)
{
  char c = str.c_str()[str.length() - 1];
  if (c != '[' && c != '{' && c != ',')
    str += cm;
}

void FirebaseJson::addBrk(std::string &str, const char *brk)
{
  char c = str.c_str()[str.length() - 1];
  if (c == ',' && (brk[0] == ']' || brk[0] == '}'))
    str[str.length() - 1] = brk[0];
  else
    str += brk;
}

void FirebaseJson::_get(const char *key, int index)
{
  _lastKey = key;
  _tokenMatch = false;
  _parseArray = index > -1;
  if (_paresRes)
  {
    int bufLen = toStdString().length() + 1;
    char *buf = new char[bufLen];
    memset(buf, 0, bufLen);
    strcpy(buf, toStdString().c_str());
    if (_jsonObj.success)
      parseToken(buf, _nextToken, _nextDept, key, index, -1, PRINT_MODE_NONE);
    delete[] buf;
    if (!_tokenMatch)
    {
      _paresRes = false;
      _jsonObj.success = false;
      resetParseResult();
    }
  }
}

void FirebaseJson::strToTk(const std::string &str, std::vector<std::string> &cont, char delim)
{
  std::size_t current, previous = 0;
  current = str.find(delim);
  std::string s;
  while (current != std::string::npos)
  {
    s = str.substr(previous, current - previous);
    trim(s);
    if (s.length() > 0)
      cont.push_back(s);
    previous = current + 1;
    current = str.find(delim, previous);
  }
  s = str.substr(previous, current - previous);
  trim(s);
  if (s.length() > 0)
    cont.push_back(s);
  std::string().swap(s);
}

void FirebaseJson::ltrim(std::string &str, const std::string &chars)
{
  str.erase(0, str.find_first_not_of(chars));
}

void FirebaseJson::rtrim(std::string &str, const std::string &chars)
{
  str.erase(str.find_last_not_of(chars) + 1);
}

void FirebaseJson::trim(std::string &str, const std::string &chars)
{
  rtrim(str, chars);
  ltrim(str, chars);
}

std::string FirebaseJson::getString()
{
  if (_jsonObj.stringValue.c_str()[0] == '"')
    return _jsonObj.stringValue.substring(1, _jsonObj.stringValue.length() - 1).c_str();
  else
    return _jsonObj.stringValue.c_str();
}

size_t FirebaseJson::getJsonObjectIteratorCount()
{
  if (_tokenCount >= 0)
    return _tokenCount;
  return 0;
}

void FirebaseJson::_parse(const char *path)
{
  _parse();
  if (!_jsonObj.success)
    return;
  _parseArray = false;
  char *et = new char[2];
  memset(et, 0, 2);
  size_t len = _pathTk.size();
  _pathNum = len;
  _pathIndex = -1;
  _arrInsIndex = -1;
  for (size_t i = 0; i < len; i++)
  {
    if (isStrTk(i))
      _get(_pathTk[i].c_str());
    else if (isArrTk(i))
      _get(et, getArrIndex(i));
    else
      _get(_pathTk[i].c_str());
    if (_tokenMatch)
      _pathIndex++;
    else
      break;
  }
  delete[] et;
}

void FirebaseJson::clearPathTk()
{
  size_t len = _pathTk.size();
  for (size_t i = 0; i < len; i++)
    std::string().swap(_pathTk[i]);
  for (size_t i = 0; i < len; i++)
    _pathTk.erase(_pathTk.end());
  _pathTk.clear();
  std::vector<std::string>().swap(_pathTk);
}

bool FirebaseJson::_compile(const char *path, int mode, const char *replace)
{
  _parse(path);
  if (!_tokenMatch && (mode == COMP_MODE_REMOVE_OBJ || mode == COMP_MODE_REMOVE_ARR))
    return false;
  if (mode != COMP_MODE_REMOVE_OBJ && mode != COMP_MODE_REMOVE_ARR)
  {
    if (_pathIndex == -1)
    {
      _pathIndex++;
      if (_jstr.length() > 0)
      {
        char *cm = new char[3];
        memset(cm, 0, 3);
        strcpy_P(cm, FirebaseJson_STR_23);
        addComma(_jstr, cm);
        delete[] cm;
      }
      addPlaceholder(_jstr, 2);
      addPathTk(_jstr, _pathIndex);
      addPlaceholder(_jstr, 3);
      if (isArrTk(_pathIndex + 1))
        addPlaceholder(_jstr, 1);
      else
        addPlaceholder(_jstr, 0);
      _parse(path);
    }
    if (_tokenCount < 0)
      return false;
    _pathIndex++;
    if (!_jsonObj.success)
    {
      if (isArrTk(_pathIndex))
      {
        mode = COMP_MODE_INSERT_ARR;
        _arrInsIndex = getArrIndex(_pathIndex);
      }
      else
        mode = COMP_MODE_INSERT_OBJ;
      _replaceToken = -1;
    }
    else
    {
      if (isArrTk(_pathIndex - 1))
      {
        mode = COMP_MODE_INSERT_ARR;
        _arrInsIndex = getArrIndex(_pathIndex - 1);
      }
      else
        mode = COMP_MODE_INSERT_OBJ;
    }
  }
  _nextToken = 0;
  _nextDept = 0;
  _tokenMatch = false;
  if (_paresRes || mode == COMP_MODE_INSERT_OBJ || mode == COMP_MODE_INSERT_ARR)
  {
    _skipNode = false;
    _insertNode = false;
    int bufLen = toStdString().length() + 1;
    char *buf = new char[bufLen];
    memset(buf, 0, bufLen);
    strcpy(buf, toStdString().c_str());
    if (((mode == COMP_MODE_REMOVE_OBJ || mode == COMP_MODE_REMOVE_ARR) && !_parseArray))
      _replaceToken--;
    if (_jsonObj.success || mode == COMP_MODE_INSERT_OBJ || mode == COMP_MODE_INSERT_ARR)
      compileToken(buf, _nextToken, _nextDept, mode, replace);
    delete[] buf;
  }
  if (mode == COMP_MODE_REMOVE_OBJ || mode == COMP_MODE_REMOVE_ARR)
  {
    if (!_paresRes)
      return false;
  }
  if (_jsonObj._bufStr.length() >= 2)
    _jstr = _jsonObj._bufStr.substr(1, _jsonObj._bufStr.length() - 2);
  _parse();
  if (mode != COMP_MODE_REMOVE_OBJ && mode != COMP_MODE_REMOVE_ARR)
    return _pathIndex == _pathNum;
  return true;
}

bool FirebaseJson::isArrTk(int index)
{
  if (index < (int)_pathTk.size())
    return _pathTk[index].c_str()[0] == '[' && _pathTk[index].c_str()[_pathTk[index].length() - 1] == ']';
  else
    return false;
}
bool FirebaseJson::isStrTk(int index)
{
  if (index < (int)_pathTk.size())
    return _pathTk[index].c_str()[0] == '"' && _pathTk[index].c_str()[_pathTk[index].length() - 1] == '"';
  else
    return false;
}

int FirebaseJson::getArrIndex(int index)
{
  int res = -1;
  if (index < (int)_pathTk.size())
  {
    res = atoi(_pathTk[index].substr(1, _pathTk[index].length() - 2).c_str());
    if (res < 0)
      res = 0;
  }
  return res;
}

bool FirebaseJson::set(const String &path)
{
  return _setNull(path.c_str());
}

bool FirebaseJson::set(const String &path, const String &value)
{
  return _setString(path.c_str(), value.c_str());
}

bool FirebaseJson::set(const String &path, const char *value)
{
  return _setString(path.c_str(), value);
}

bool FirebaseJson::set(const String &path, int value)
{
  return _setInt(path.c_str(), value);
}

bool FirebaseJson::set(const String &path, double value)
{
  return _setDouble(path.c_str(), value);
}

bool FirebaseJson::set(const String &path, bool value)
{
  return _setBool(path.c_str(), value);
}

bool FirebaseJson::set(const String &path, FirebaseJson &json)
{
  return _setJson(path.c_str(), &json);
}

bool FirebaseJson::set(const String &path, FirebaseJsonArray &arr)
{
  return _setArray(path.c_str(), &arr);
}

template <typename T>
bool FirebaseJson::set(const String &path, T value)
{
  if (std::is_same<T, int>::value)
    return _setInt(path, value);
  else if (std::is_same<T, double>::value)
    return _setDouble(path, value);
  else if (std::is_same<T, bool>::value)
    return _setBool(path, value);
  else if (std::is_same<T, const char *>::value)
    return _setString(path, value);
  else if (std::is_same<T, FirebaseJson &>::value)
    return _setJson(path, &value);
  else if (std::is_same<T, FirebaseJsonArray &>::value)
    return _setArray(path, &value);
}

bool FirebaseJson::_setString(const std::string &path, const std::string &value)
{
  char *tmp = new char[value.length() + 10];
  memset(tmp, 0, value.length() + 10);
  strcpy_P(tmp, FirebaseJson_STR_2);
  strcat(tmp, value.c_str());
  strcat_P(tmp, FirebaseJson_STR_2);
  bool res = _set(path.c_str(), COMP_MODE_REPLACE_OBJ, tmp);
  delete[] tmp;
  std::string().swap(_jsonObj._bufStr);
  return res;
}

bool FirebaseJson::_setInt(const std::string &path, int value)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_4);
  sprintf(tmp, b, value);
  delete[] b;
  bool res = _set(path.c_str(), COMP_MODE_REPLACE_OBJ, tmp);
  delete[] tmp;
  std::string().swap(_jsonObj._bufStr);
  return res;
}

bool FirebaseJson::_setDouble(const std::string &path, double value)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_5);
  sprintf(tmp, b, value);
  int i = strlen(tmp) - 1;
  while (tmp[i] == '0' && i > 0)
    i--;
  if (i < (int)strlen(tmp) - 1)
    tmp[i + 1] = '\0';
  delete[] b;
  bool res = _set(path.c_str(), COMP_MODE_REPLACE_OBJ, tmp);
  delete[] tmp;
  std::string().swap(_jsonObj._bufStr);
  return res;
}

bool FirebaseJson::_setBool(const std::string &path, bool value)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  if (value)
    strcat_P(tmp, FirebaseJson_STR_7);
  else
    strcat_P(tmp, FirebaseJson_STR_6);
  bool res = _set(path.c_str(), COMP_MODE_REPLACE_OBJ, tmp);
  delete[] tmp;
  std::string().swap(_jsonObj._bufStr);
  return res;
}

bool FirebaseJson::_setNull(const std::string &path)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  strcat_P(tmp, FirebaseJson_STR_18);
  bool res = _set(path.c_str(), COMP_MODE_REPLACE_OBJ, tmp);
  delete[] tmp;
  std::string().swap(_jsonObj._bufStr);
  return res;
}

bool FirebaseJson::_setJson(const std::string &path, FirebaseJson *json)
{
  return _set(path.c_str(), COMP_MODE_REPLACE_OBJ, json->toStdString().c_str());
}

bool FirebaseJson::_setArray(const std::string &path, FirebaseJsonArray *arr)
{
  return _set(path.c_str(), COMP_MODE_REPLACE_OBJ, arr->toStdString().c_str());
}

bool FirebaseJson::_set(const char *path, int mode, const char *data)
{
  bool res = false;
  clearPathTk();
  strToTk(path, _pathTk, '/');
  for (size_t i = 0; i < _pathTk.size(); i++)
  {
    _maxRound++;
    if (isArrTk(i))
      _maxRound += getArrIndex(i);
  }
  while (!res && _maxRound > 0)
  {
    res = _compile(path, mode, data);
    _maxRound--;
  }
  clearPathTk();
  std::string().swap(_jsonObj._bufStr);
  _tokens.reset();
  _tokens.release();
  _tokens = nullptr;
  return _jsonObj.success;
}

bool FirebaseJson::remove(const String &path)
{
  char *tmp = new char[2];
  memset(tmp, 0, 2);
  clearPathTk();
  strToTk(path.c_str(), _pathTk, '/');
  _compile(path.c_str(), COMP_MODE_REMOVE_OBJ, tmp);
  delete[] tmp;
  std::string().swap(_jsonObj._bufStr);
  clearPathTk();
  _tokens.reset();
  _tokens.release();
  _tokens = nullptr;
  return _jsonObj.success;
}

FirebaseJson &FirebaseJson::jsonObjectiterator(size_t &index, String &key, String &value)
{
  if (_paresRes)
  {
    int bufLen = toStdString().length() + 1;
    char *k = new char[200];
    char *v = new char[bufLen];
    char *buf = new char[bufLen];
    memset(buf, 0, bufLen);
    strcpy(buf, toStdString().c_str());
    if (index == 0)
      index = 1;
    if (index >= 1)
    {
      _jsonObj.success = false;
      resetParseResult();
      jsmntok_t *h = &_tokens.get()[index];
      jsmntok_t *g = &_tokens.get()[index + 1];
      memset(k, 0, 200);
      strncpy(k, buf + h->start, h->end - h->start);
      key = k;
      memset(v, 0, bufLen);
      strncpy(v, buf + g->start, g->end - g->start);
      value = v;
      _jsonObj.stringValue = v;
      _jsonObj._type = g->type;
      _jsonObj._k_start = h->start;
      _jsonObj._start = g->start;
      _jsonObj._end = g->end;
      _jsonObj.success = true;
      if (g->type == JSMN_ARRAY)
        index += g->size + 1;
      else
        index++;
      setElementType();
    }
    delete[] buf;
    delete[] k;
    delete[] v;
  }
  return *this;
}

void FirebaseJson::resetParseResult()
{
  _jsonObj._type = 0;
  _jsonObj.type = "";
  _jsonObj.typeNum = 0;
  _jsonObj.stringValue = "";
  _jsonObj._bufStr = "";
  _jsonObj.intValue = 0;
  _jsonObj.doubleValue = 0;
  _jsonObj.boolValue = false;
  _jsonObj._o_index = -1;
  _jsonObj._a_index = -1;
}

void FirebaseJson::setElementType()
{
  bool typeSet = false;
  char *buf = new char[20];
  memset(buf, 0, 20);
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  switch (_jsonObj._type)
  {
  case JSMN_UNDEFINED:
    strcpy_P(buf, FirebaseJson_STR_19);
    _jsonObj.typeNum = JSON_UNDEFINED;
    break;
  case JSMN_OBJECT:
    strcpy_P(buf, FirebaseJson_STR_16);
    _jsonObj.typeNum = JSON_OBJECT;
    break;
  case JSMN_ARRAY:
    strcpy_P(buf, FirebaseJson_STR_17);
    _jsonObj.typeNum = JSON_ARRAY;
    break;
  case JSMN_STRING:
    strcpy_P(buf, FirebaseJson_STR_12);
    _jsonObj.typeNum = JSON_STRING;
    break;
  case JSMN_PRIMITIVE:
    strcpy_P(tmp, FirebaseJson_STR_7);
    if (strcmp(_jsonObj.stringValue.c_str(), tmp) == 0)
    {
      typeSet = true;
      strcpy_P(buf, FirebaseJson_STR_15);
      _jsonObj.typeNum = JSON_BOOL;
      _jsonObj.boolValue = true;
      _jsonObj.doubleValue = 1.0;
      _jsonObj.intValue = 1;
    }
    else
    {
      memset(tmp, 0, 20);
      strcpy_P(tmp, FirebaseJson_STR_6);
      if (strcmp(_jsonObj.stringValue.c_str(), tmp) == 0)
      {
        typeSet = true;
        strcpy_P(buf, FirebaseJson_STR_15);
        _jsonObj.typeNum = JSON_BOOL;
        _jsonObj.boolValue = false;
        _jsonObj.doubleValue = 0.0;
        _jsonObj.intValue = 0;
      }
    }
    memset(tmp, 0, 20);
    strcpy_P(tmp, FirebaseJson_STR_20);
    if (!typeSet && strpos(_jsonObj.stringValue.c_str(), tmp, 0) != -1)
    {
      typeSet = true;
      strcpy_P(buf, FirebaseJson_STR_14);
      _jsonObj.typeNum = JSON_DOUBLE;
      _jsonObj.doubleValue = atof(_jsonObj.stringValue.c_str());
      _jsonObj.intValue = atoi(_jsonObj.stringValue.c_str());
      _jsonObj.boolValue = atof(_jsonObj.stringValue.c_str()) > 0 ? true : false;
    }
    memset(tmp, 0, 20);
    strcpy_P(tmp, FirebaseJson_STR_18);
    if (!typeSet && strcmp(_jsonObj.stringValue.c_str(), tmp) == 0)
    {
      typeSet = true;
      strcpy_P(buf, FirebaseJson_STR_18);
      _jsonObj.typeNum = JSON_NULL;
    }
    if (!typeSet)
    {
      typeSet = true;
      double d = atof(_jsonObj.stringValue.c_str());
      if (d > 0x7fffffff)
      {
        strcpy_P(buf, FirebaseJson_STR_14);
        _jsonObj.doubleValue = d;
        _jsonObj.intValue = atoi(_jsonObj.stringValue.c_str());
        _jsonObj.boolValue = atof(_jsonObj.stringValue.c_str()) > 0 ? true : false;
        _jsonObj.typeNum = JSON_DOUBLE;
      }
      else
      {
        _jsonObj.intValue = atoi(_jsonObj.stringValue.c_str());
        _jsonObj.doubleValue = atof(_jsonObj.stringValue.c_str());
        _jsonObj.boolValue = atof(_jsonObj.stringValue.c_str()) > 0 ? true : false;
        strcpy_P(buf, FirebaseJson_STR_13);
        _jsonObj.typeNum = JSON_INT;
      }
    }
    break;
  default:
    break;
  }
  _jsonObj.type = buf;
  delete[] buf;
  delete[] tmp;
}

int FirebaseJson::strpos(const char *haystack, const char *needle, int offset)
{
  size_t len = strlen(haystack);
  char *_haystack = new char[len];
  memset(_haystack, 0, len);
  strncpy(_haystack, haystack + offset, strlen(haystack) - offset);
  char *p = strstr(_haystack, needle);
  int r = -1;
  if (p)
    r = p - _haystack + offset;
  delete[] _haystack;
  return r;
}

int FirebaseJson::rstrpos(const char *haystack, const char *needle, int offset)
{
  size_t len = strlen(haystack);
  char *_haystack = new char[len];
  memset(_haystack, 0, len);
  strncpy(_haystack, haystack + offset, len - offset);
  char *p = rstrstr(_haystack, needle);
  int r = -1;
  if (p)
    r = p - _haystack + offset;
  delete[] _haystack;
  return r;
}

char *FirebaseJson::rstrstr(const char *haystack, const char *needle)
{
  size_t needle_length = strlen(needle);
  const char *haystack_end = haystack + strlen(haystack) - needle_length;
  const char *p;
  size_t i;
  for (p = haystack_end; p >= haystack; --p)
  {
    for (i = 0; i < needle_length; ++i)
    {
      if (p[i] != needle[i])
        goto next;
    }
    return (char *)p;
  next:;
  }
  return 0;
}

FirebaseJsonData FirebaseJson::parseResult()
{
  return _jsonObj;
}

FirebaseJsonArray::FirebaseJsonArray(){};
FirebaseJsonArray::~FirebaseJsonArray()
{
  std::string().swap(_jstr);
};

FirebaseJsonArray &FirebaseJsonArray::add()
{
  _addNull();
  return *this;
}

FirebaseJsonArray &FirebaseJsonArray::add(const String &value)
{
  _addString(value.c_str());
  return *this;
}
FirebaseJsonArray &FirebaseJsonArray::add(const char *value)
{
  _addString(value);
  return *this;
}
FirebaseJsonArray &FirebaseJsonArray::add(int value)
{
  _addInt(value);
  return *this;
}

FirebaseJsonArray &FirebaseJsonArray::add(double value)
{
  _addDouble(value);
  return *this;
}

FirebaseJsonArray &FirebaseJsonArray::add(bool value)
{
  _addBool(value);
  return *this;
}

FirebaseJsonArray &FirebaseJsonArray::add(FirebaseJson &json)
{
  _addJson(&json);
  return *this;
}

FirebaseJsonArray &FirebaseJsonArray::add(FirebaseJsonArray &arr)
{
  _addArray(&arr);
  return *this;
}

template <typename T>
FirebaseJsonArray &FirebaseJsonArray::add(T value)
{
  if (std::is_same<T, int>::value)
    _addInt(value);
  else if (std::is_same<T, double>::value)
    _addDouble(value);
  else if (std::is_same<T, bool>::value)
    _addBool(value);
  else if (std::is_same<T, const char *>::value)
    _addString(value);
  else if (std::is_same<T, FirebaseJson &>::value)
    _addJson(&value);
  else if (std::is_same<T, FirebaseJsonArray &>::value)
    _addArray(&value);
  return *this;
}

void FirebaseJsonArray::_addString(const std::string &value)
{
  _arrLen++;
  _json.addArrayStr(value.c_str(), value.length(), true);
}

void FirebaseJsonArray::_addInt(int value)
{
  _arrLen++;
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_4);
  sprintf(buf, b, value);
  _json.addArrayStr(buf, 60, false);
  delete[] b;
  delete[] buf;
}

void FirebaseJsonArray::_addDouble(double value)
{
  _arrLen++;
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_5);
  sprintf(buf, b, value);
  _json.addArrayStr(buf, 60, false);
  delete[] b;
  delete[] buf;
}

void FirebaseJsonArray::_addBool(bool value)
{
  _arrLen++;
  size_t bufSize = 20;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  if (value)
    strcat_P(buf, FirebaseJson_STR_7);
  else
    strcat_P(buf, FirebaseJson_STR_6);
  _json.addArrayStr(buf, bufSize, false);
  delete[] buf;
}

void FirebaseJsonArray::_addNull()
{
  _arrLen++;
  size_t bufSize = 20;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  strcat_P(buf, FirebaseJson_STR_18);
  _json.addArrayStr(buf, bufSize, false);
  delete[] buf;
}

void FirebaseJsonArray::_addJson(FirebaseJson *json)
{
  _arrLen++;
  _json.addArrayStr(json->toStdString().c_str(), json->toStdString().length(), false);
}

void FirebaseJsonArray::_addArray(FirebaseJsonArray *arr)
{
  _arrLen++;
  _json.addArrayStr(arr->toString().c_str(), arr->toString().length(), false);
}

bool FirebaseJsonArray::get(FirebaseJsonData &jsonObj, const String &path)
{
  return _get(jsonObj, path.c_str());
}

bool FirebaseJsonArray::get(FirebaseJsonData &jsonObj, int index)
{
  char *tmp = new char[50];
  std::string path = "";
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_10);
  path += tmp;
  memset(tmp, 0, 50);
  itoa(index, tmp, 10);
  path += tmp;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_11);
  path += tmp;
  bool ret = _get(jsonObj, path.c_str());
  std::string().swap(path);
  delete[] tmp;
  return ret;
}

bool FirebaseJsonArray::_get(FirebaseJsonData &jsonObj, const char *path)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_21);
  _jstr = _json.toStdString(false);
  _json._jstr = tmp;
  _json._jstr += _jstr;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_26);
  std::string path2 = tmp;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_27);
  path2 += tmp;
  path2 += path;
  _json.clearPathTk();
  _json.strToTk(path2.c_str(), _json._pathTk, '/');
  if (!_json.isArrTk(1))
  {
    _json._jsonObj.success = false;
    goto ex_;
  }
  if (_json.getArrIndex(1) < 0)
  {
    _json._jsonObj.success = false;
    goto ex_;
  }
  _json._parse(path2.c_str());
  if (_json._jsonObj.success)
  {
    _json._jstr = _jstr.substr(1, _jstr.length() - 2).c_str();
    if (_json._jsonObj._type == JSMN_STRING)
      _json._jsonObj.stringValue = _json._jsonObj.stringValue.substring(1, _json._jsonObj.stringValue.length() - 1);
    jsonObj = _json._jsonObj;
  }
ex_:
  delete[] tmp;
  _json.clearPathTk();
  _json._tokens.reset();
  _json._tokens.release();
  _json._tokens = nullptr;
  return _json._jsonObj.success;
}

size_t FirebaseJsonArray::size()
{
  return _arrLen;
}

String FirebaseJsonArray::toString(bool prettify)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  char *et = new char[2];
  memset(et, 0, 2);
  strcpy_P(tmp, FirebaseJson_STR_21);
  _jstr = _json.toStdString(false);
  _json._jstr = tmp;
  _json._jstr += _jstr;
  memset(tmp, 0, 20);
  strcpy_P(tmp, FirebaseJson_STR_26);
  _json._parse();
  _json._get(tmp);
  if (_json._paresRes)
  {
    _json._tokenMatch = false;
    int bufLen = _json.toStdString().length() + 1;
    char *buf = new char[bufLen];
    memset(buf, 0, bufLen);
    strcpy(buf, _json.toStdString().c_str());
    _json._nextDept = 0;
    int mode = prettify ? PRINT_MODE_PRETTY : PRINT_MODE_PLAIN;
    int ntk = _json._nextToken;
    int ndp = _json._nextDept;
    if (_json._jsonObj.success)
    {
      _json.parseToken(buf, _json._nextToken, _json._nextDept, et, -1, -1, PRINT_MODE_PLAIN);
      _json._jstr = _json._jsonObj._bufStr.substr(1, _json._jsonObj._bufStr.length() - 2);
      if (mode == PRINT_MODE_PRETTY)
      {
        std::string().swap(_json._jsonObj._bufStr);
        _json.parseToken(buf, ntk, ndp, et, -1, -1, mode);
      }
    }
    delete[] buf;
    delete[] et;
    delete[] tmp;
    _json._tokens.reset();
    _json._tokens.release();
    _json._tokens = nullptr;
    return _json._jsonObj._bufStr.c_str();
  }
  else
    _json._jstr = _jstr;
  delete[] et;
  delete[] tmp;
  _json._tokens.reset();
  _json._tokens.release();
  _json._tokens = nullptr;
  return std::string().c_str();
}

void FirebaseJsonArray::setEmptyArr(std::string &str)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  strcpy_P(tmp, FirebaseJson_STR_10);
  strcat_P(tmp, FirebaseJson_STR_11);
  str += tmp;
  delete[] tmp;
}

FirebaseJsonArray &FirebaseJsonArray::clear()
{
  _json.clear();
  std::string().swap(_jstr);
  return *this;
}
void FirebaseJsonArray::_set2(int index, const char *value, bool isStr)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_10);
  std::string path = tmp;
  memset(tmp, 0, 50);
  itoa(index, tmp, 10);
  path += tmp;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_11);
  path += tmp;
  _set(path.c_str(), value, isStr);
  std::string().swap(path);
  delete[] tmp;
}

void FirebaseJsonArray::_set(const char *path, const char *value, bool isStr)
{
  _json._jsonObj.success = false;
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_21);
  char *et = new char[2];
  memset(et, 0, 2);
  _jstr = _json.toStdString(false);
  _json._jstr = tmp;
  _json._jstr += _jstr;
  char *tmp2 = new char[strlen(value) + 10];
  memset(tmp2, 0, strlen(value) + 10);
  if (isStr)
    strcpy_P(tmp2, FirebaseJson_STR_2);
  strcat(tmp2, value);
  if (isStr)
    strcat_P(tmp2, FirebaseJson_STR_2);
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_26);
  std::string path2 = tmp;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_27);
  path2 += tmp;
  path2 += path;
  bool res = false;
  _json.clearPathTk();
  _json.strToTk(path2, _json._pathTk, '/');
  if (!_json.isArrTk(1))
    goto ex_2;
  if (_json.getArrIndex(1) < 0)
    goto ex_2;
  for (size_t i = 0; i < _json._pathTk.size(); i++)
  {
    _json._maxRound++;
    if (_json.isArrTk(i))
      _json._maxRound += _json.getArrIndex(i);
  }
  while (!res && _json._maxRound > 0)
  {
    res = _json._compile(path2.c_str(), COMP_MODE_REPLACE_ARR, tmp2);
    _json._maxRound--;
  }
  delete[] tmp2;
  std::string().swap(path2);
  if (_json._jsonObj.success)
  {
    _json._parse();
    memset(tmp, 0, 50);
    strcpy_P(tmp, FirebaseJson_STR_26);
    _json._get(tmp);
    _jstr = _json.getString();
    _json._jstr = _jstr.substr(1, _jstr.length() - 2);
    if (_json._jsonObj._type == JSMN_STRING)
      _json._jsonObj.stringValue = _json._jsonObj.stringValue.substring(1, _json._jsonObj.stringValue.length() - 1);
    _json._get(et, 0);
    _arrLen = _json._jsonObj._len;
  }
  else
    _json._jstr = _jstr.substr(1, _jstr.length() - 2);
ex_2:
  delete[] tmp;
  delete[] et;
  std::string().swap(_json._jsonObj._bufStr);
  std::string().swap(_jstr);
  _json.clearPathTk();
  _json._tokens.reset();
  _json._tokens.release();
  _json._tokens = nullptr;
}

bool FirebaseJsonArray::set(int index)
{
  return _setNull(index);
}

bool FirebaseJsonArray::set(const String &path)
{
  return _setNull(path);
}

bool FirebaseJsonArray::set(int index, const String &value)
{
  return _setString(index, value.c_str());
}

bool FirebaseJsonArray::set(const String &path, const String &value)
{
  return _setString(path, value.c_str());
}

bool FirebaseJsonArray::set(int index, const char *value)
{
  return _setString(index, value);
}

bool FirebaseJsonArray::set(const String &path, const char *value)
{
  return _setString(path, value);
}

bool FirebaseJsonArray::set(int index, int value)
{
  return _setInt(index, value);
}

bool FirebaseJsonArray::set(const String &path, int value)
{
  return _setInt(path, value);
}

bool FirebaseJsonArray::set(int index, double value)
{
  return _setDouble(index, value);
}

bool FirebaseJsonArray::set(const String &path, double value)
{
  return _setDouble(path, value);
}

bool FirebaseJsonArray::set(int index, bool value)
{
  return _setBool(index, value);
}

bool FirebaseJsonArray::set(const String &path, bool value)
{
  return _setBool(path, value);
}

bool FirebaseJsonArray::set(int index, FirebaseJson &json)
{
  return _setJson(index, &json);
}

bool FirebaseJsonArray::set(const String &path, FirebaseJson &json)
{
  return _setJson(path, &json);
}

bool FirebaseJsonArray::set(int index, FirebaseJsonArray &arr)
{
  return _setArray(index, &arr);
}

bool FirebaseJsonArray::set(const String &path, FirebaseJsonArray &arr)
{
  return _setArray(path, &arr);
}

template <typename T>
bool FirebaseJsonArray::set(int index, T value)
{
  if (std::is_same<T, int>::value)
    return _setInt(index, value);
  else if (std::is_same<T, double>::value)
    return _setDouble(index, value);
  else if (std::is_same<T, bool>::value)
    return _setBool(index, value);
  else if (std::is_same<T, const char *>::value)
    return _setString(index, value);
  else if (std::is_same<T, FirebaseJson &>::value)
    return _setJson(index, &value);
  else if (std::is_same<T, FirebaseJsonArray &>::value)
    return _setArray(index, &value);
}

template <typename T>
bool FirebaseJsonArray::set(const String &path, T value)
{
  if (std::is_same<T, int>::value)
    return _setInt(path, value);
  else if (std::is_same<T, double>::value)
    return _setDouble(path, value);
  else if (std::is_same<T, bool>::value)
    return _setBool(path, value);
  else if (std::is_same<T, const char *>::value)
    return _setString(path, value);
  else if (std::is_same<T, FirebaseJson &>::value)
    return _setJson(path, &value);
  else if (std::is_same<T, FirebaseJsonArray &>::value)
    return _setArray(path, &value);
}

bool FirebaseJsonArray::_setString(int index, const std::string &value)
{
  _set2(index, value.c_str(), true);
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setString(const String &path, const std::string &value)
{
  _set(path.c_str(), value.c_str(), true);
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setInt(int index, int value)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_4);
  sprintf(tmp, b, value);
  delete[] b;
  _set2(index, tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setInt(const String &path, int value)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_4);
  sprintf(tmp, b, value);
  delete[] b;
  _set(path.c_str(), tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setDouble(int index, double value)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_5);
  sprintf(tmp, b, value);
  int i = strlen(tmp) - 1;
  while (tmp[i] == '0' && i > 0)
    i--;
  if (i < (int)strlen(tmp) - 1)
    tmp[i + 1] = '\0';
  delete[] b;
  _set2(index, tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setDouble(const String &path, double value)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  char *b = new char[10];
  memset(b, 0, 10);
  strcat_P(b, FirebaseJson_STR_5);
  sprintf(tmp, b, value);
  int i = strlen(tmp) - 1;
  while (tmp[i] == '0' && i > 0)
    i--;
  if (i < (int)strlen(tmp) - 1)
    tmp[i + 1] = '\0';
  delete[] b;
  _set(path.c_str(), tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setBool(int index, bool value)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  if (value)
    strcat_P(tmp, FirebaseJson_STR_7);
  else
    strcat_P(tmp, FirebaseJson_STR_6);
  _set2(index, tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setBool(const String &path, bool value)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  if (value)
    strcat_P(tmp, FirebaseJson_STR_7);
  else
    strcat_P(tmp, FirebaseJson_STR_6);
  _set(path.c_str(), tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setNull(int index)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  strcat_P(tmp, FirebaseJson_STR_18);
  _set2(index, tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setNull(const String &path)
{
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  strcat_P(tmp, FirebaseJson_STR_18);
  _set(path.c_str(), tmp, false);
  delete[] tmp;
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setJson(int index, FirebaseJson *json)
{
  _set2(index, json->toStdString().c_str(), false);
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setJson(const String &path, FirebaseJson *json)
{
  _set(path.c_str(), json->toStdString().c_str(), false);
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setArray(int index, FirebaseJsonArray *arr)
{
  _set2(index, arr->toStdString().c_str(), false);
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::_setArray(const String &path, FirebaseJsonArray *arr)
{
  _set(path.c_str(), arr->toStdString().c_str(), false);
  return _json._jsonObj.success;
}

bool FirebaseJsonArray::remove(int index)
{
  char *tmp = new char[50];
  std::string path = "";
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_10);
  path += tmp;
  memset(tmp, 0, 50);
  itoa(index, tmp, 10);
  path += tmp;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_11);
  path += tmp;
  bool ret = _remove(path.c_str());
  std::string().swap(path);
  delete[] tmp;
  return ret;
}

bool FirebaseJsonArray::remove(const String &path)
{
  return _remove(path.c_str());
}

bool FirebaseJsonArray::_remove(const char *path)
{
  char *tmp = new char[50];
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_21);
  char *et = new char[2];
  memset(et, 0, 2);
  _jstr = _json.toStdString(false);
  _json._jstr = tmp;
  _json._jstr += _jstr;
  char *tmp2 = new char[2];
  memset(tmp2, 0, 2);
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_26);
  std::string path2 = tmp;
  memset(tmp, 0, 50);
  strcpy_P(tmp, FirebaseJson_STR_27);
  path2 += tmp;
  path2 += path;
  _json.clearPathTk();
  _json.strToTk(path2.c_str(), _json._pathTk, '/');
  if (!_json.isArrTk(1))
  {
    _json._jsonObj.success = false;
    goto ex_1;
  }
  if (_json.getArrIndex(1) < 0)
  {
    _json._jsonObj.success = false;
    goto ex_1;
  }
  _json._compile(path2.c_str(), COMP_MODE_REMOVE_OBJ, tmp2);
  delete[] tmp2;
  std::string().swap(path2);
  if (_json._jsonObj.success)
  {
    _json._parse();
    memset(tmp, 0, 50);
    strcpy_P(tmp, FirebaseJson_STR_26);
    _json._get(tmp);
    _jstr = _json.getString();
    _json._jstr = _jstr.substr(1, _jstr.length() - 2);
    if (_json._jsonObj._type == JSMN_STRING)
      _json._jsonObj.stringValue = _json._jsonObj.stringValue.substring(1, _json._jsonObj.stringValue.length() - 1);
    _json._get(et, 0);
    _arrLen = _json._jsonObj._len;
  }
  else
    _json._jstr = _jstr.substr(1, _jstr.length() - 2);
ex_1:
  std::string().swap(_jstr);
  std::string().swap(_json._jsonObj._bufStr);
  delete[] tmp;
  delete[] et;
  _json.clearPathTk();
  _json._tokens.reset();
  _json._tokens.release();
  _json._tokens = nullptr;
  return _json._jsonObj.success;
}

std::string FirebaseJsonArray::toStdString()
{
  return _json.toStdString(false);
}

FirebaseJsonData::FirebaseJsonData()
{
}

FirebaseJsonData::~FirebaseJsonData()
{
  std::string().swap(_bufStr);
}

bool FirebaseJsonData::getArray(FirebaseJsonArray &jsonArray)
{
  if (typeNum != JSON_ARRAY || !success)
    return false;
  char *tmp = new char[20];
  memset(tmp, 0, 20);
  char *et = new char[2];
  memset(et, 0, 2);
  strcpy_P(tmp, FirebaseJson_STR_21);
  jsonArray._jstr = jsonArray._json.toStdString(false);
  jsonArray._json._jstr = tmp;
  jsonArray._json._jstr += stringValue.c_str();
  memset(tmp, 0, 20);
  strcpy_P(tmp, FirebaseJson_STR_26);
  jsonArray._json._parse();
  jsonArray._json._get(tmp);
  jsonArray._json._jstr = jsonArray._json._jsonObj.stringValue.substring(1, jsonArray._json._jsonObj.stringValue.length() - 1).c_str();
  jsonArray._json._get(et, 0);
  jsonArray._arrLen = jsonArray._json._jsonObj._len;
  delete[] tmp;
  delete[] et;
  return jsonArray._json._jsonObj.success;
}

bool FirebaseJsonData::getJSON(FirebaseJson &json)
{
  if (typeNum != JSON_OBJECT || !success)
    return false;
  json.setJsonData(stringValue);
  json._parse();
  return json._jsonObj.success;
}