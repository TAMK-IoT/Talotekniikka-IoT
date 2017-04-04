/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "IOT_WriteData.h"
#include "IOT_defines.h"
#include "IOT_Base64.h"
#include <string.h>
#include <sstream>
#include <time.h>

using namespace IOTAPI;

const uint32_t IOT_WriteData::MAX_STATIC_SIZE = 8;

IOT_WriteData::IOT_WriteData(): m_name(""), m_path(""), m_unit(""),
    m_dataType(IOT_no_type), m_dynValue(NULL), m_valSize(0), m_timeStampMs(0)
{}

IOT_WriteData::~IOT_WriteData()
{
    ClearDynamic();
}

bool IOT_WriteData::SetName(const std::string &name)
{
    if(name.length() > IOTAPI::IOTAPI_MAX_NAME_LEN) {
        return false;
    }

    m_name = name;
    return true;
}

bool IOT_WriteData::SetPath(const std::string& path)
{
    uint32_t pathDepth = 0;

    for (uint32_t i = 0; i < path.length(); ++i)
    {
        if (path[i] == '/' && i > 0) {
            pathDepth++;
        }

        //Don't accept empty component in the path
        if(i < (path.length()-2) && path[i] == '/' && path[i+1] == '/')
        {
            return false;
        }
    }

    if(path.length() > IOTAPI::IOTAPI_MAX_PATH_LEN
       || path.find_first_not_of("0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ/") != std::string::npos
       || pathDepth > IOTAPI::IOTAPI_MAX_PATH_DEPTH) {
        return false;
    }

    m_path = path;
    return true;
}

bool IOT_WriteData::SetUnit(const std::string& unit)
{
    if(unit.length() > IOTAPI::IOTAPI_MAX_UNIT_LEN) {
        return false;
    }

    m_unit = unit;
    return true;
}

void IOT_WriteData::SetValue(const std::string& value)
{
    ClearDynamic();
    if(value.length() > MAX_STATIC_SIZE)
    {
        m_dynValue = new(std::nothrow) uint8_t[value.length()];
        memcpy(m_dynValue, value.c_str(), value.length());
    } else {
        memcpy(m_value, value.c_str(), value.length());
    }

    m_valSize = value.length();
    m_dataType = IOT_string;
}

void IOT_WriteData::SetValue(const char* value)
{
    SetValue(std::string(value));
}

void IOT_WriteData::SetValue(double value)
{
    ClearDynamic();
    *(double*)m_value = value;
    m_valSize = sizeof(double);
    m_dataType = IOT_double;
}

void IOT_WriteData::SetValue(int64_t value)
{
    ClearDynamic();
    *(int64_t*)m_value = value;
    m_valSize = sizeof(int64_t);
    m_dataType = IOT_long;
}

void IOT_WriteData::SetValue(bool value)
{
    ClearDynamic();
    *(bool*)m_value = value;
    m_valSize = sizeof(bool);
    m_dataType = IOT_bool;
}

void IOT_WriteData::SetValue(uint8_t *value, uint32_t size)
{
    ClearDynamic();
    if(size > MAX_STATIC_SIZE)
    {
        m_dynValue = new(std::nothrow) uint8_t[size];
        memcpy(m_dynValue, value, size);
    } else {
        memcpy(m_value, value, size);
    }

    m_valSize = size;
    m_dataType = IOT_binary;
}

void IOT_WriteData::SetTimeMs(uint64_t timeMs)
{
    m_timeStampMs = timeMs;
}

void IOT_WriteData::SetTimeToNow()
{
    struct timespec tv;
    if(clock_gettime(CLOCK_REALTIME, &tv) == 0) {
        m_timeStampMs = ((uint64_t)tv.tv_sec * 1000llu) + ((uint64_t)tv.tv_nsec / 1000000llu);
    }
}

bool IOT_WriteData::ToJSON(std::string& json) const
{
    Json::Value jsonVal;
    Json::FastWriter fastWriter;

    if(!ToJSON(jsonVal)) {
        return false;
    }

    json = fastWriter.write(jsonVal);
    return true;
}

bool IOT_WriteData::ToJSON(Json::Value& json) const
{
    if(m_name.empty() || m_dataType == IOT_no_type || m_valSize == 0) {
        return false;
    }

    json["name"] = m_name;

    if(!m_path.empty()) {
       json["path"] = m_path;
    }

    const uint8_t* data = getDataPointer();
    switch(m_dataType)
    {
    case IOT_double:
        json["v"] = *((double*)data);
        json["dataType"] = "double";
        break;
    case IOT_long:
        json["v"] = static_cast<Json::Int64>( *((int64_t*)data) );
        json["dataType"] = "long";
        break;
    case IOT_bool:
        json["v"] = *((bool*)data);
        json["dataType"] = "boolean";
        break;
    case IOT_string:
        json["v"] = std::string((char*)data, m_valSize);
        json["dataType"] = "string";
        break;
    case IOT_binary:
        json["v"] = IOT_Base64::encode(data, m_valSize);
        json["dataType"] = "binary";
        break;
    case IOT_no_type:
    default:
        break;
    }

    if(m_timeStampMs != 0) {
        json["ts"] = m_timeStampMs;
    }

    if(!m_unit.empty()) {
        json["unit"] = m_unit;
    }

    return true;
}

void IOT_WriteData::ClearDynamic()
{
    if(m_valSize > MAX_STATIC_SIZE && m_dynValue != NULL) {
        delete[] m_dynValue;
        m_dynValue = NULL;
        m_valSize = 0;
        m_dataType = IOT_no_type;
    }
}

const uint8_t* IOT_WriteData::getDataPointer() const
{
    if(m_valSize > MAX_STATIC_SIZE) {
        return m_dynValue;
    } else {
        return m_value;
    }
}

IOT_WriteData& IOT_WriteData::operator= (const IOT_WriteData& other)
{
    return Assign(other);
}

IOT_WriteData::IOT_WriteData(const IOT_WriteData& other)
{
    m_valSize = 0;
    Assign(other);
}

IOT_WriteData::IOT_WriteData(IOT_WriteData& other)
{
    m_valSize = 0;
    Assign(other);
}

IOT_WriteData& IOT_WriteData::Assign(const IOT_WriteData& other)
{
    if (this == &other) {
        return *this;
    }

    ClearDynamic();
    m_name        = other.m_name;
    m_path        = other.m_path;
    m_unit        = other.m_unit;
    m_dataType    = other.m_dataType;
    m_timeStampMs = other.m_timeStampMs;
    m_valSize     = other.m_valSize;

    if(m_valSize <= MAX_STATIC_SIZE) {
        memcpy(m_value, other.m_value, m_valSize);
    }
    else {
        m_dynValue = new (std::nothrow) uint8_t[m_valSize];
        if(m_dynValue == NULL) {
             m_valSize = 0;
        } else {
            memcpy(m_dynValue, other.m_dynValue, m_valSize);
        }
    }

    return *this;
}
