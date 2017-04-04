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

#include "IOT_RegDevice.h"
#include "IOT_defines.h"
#include <stdio.h>
#include "json/json.h"

IOT_RegDevice::IOT_RegDevice(): m_name(""), m_manufacturer(""), m_type(""), m_description("") {}

IOT_RegDevice::~IOT_RegDevice() {}

bool IOT_RegDevice::SetName(const std::string &name)
{
    if(name.empty() || name.length() > IOTAPI::IOTAPI_MAX_NAME_LEN) {
        return false;
    }

    m_name = name;
    return true;
}

std::string IOT_RegDevice::GetName() const
{
    return m_name;
}

bool IOT_RegDevice::SetManufacturer(const std::string &manufacturer)
{
    if(manufacturer.empty() || manufacturer.length() > IOTAPI::IOTAPI_MAX_NAME_LEN) {
        return false;
    }

    m_manufacturer = manufacturer;
    return true;
}

std::string IOT_RegDevice::GetManufacturer() const
{
    return m_manufacturer;
}

bool IOT_RegDevice::SetType(const std::string &type)
{
    if(type.empty() || type.length() > IOTAPI::IOTAPI_MAX_NAME_LEN) {
        return false;
    }

    m_type = type;
    return true;
}

std::string IOT_RegDevice::GetType() const
{
    return m_type;
}

bool IOT_RegDevice::SetDescription(const std::string &desc)
{
    if(desc.empty() || desc.length() > IOTAPI::IOTAPI_MAX_DESCRIPTION_LEN) {
        return false;
    }

    m_description = desc;
    return true;
}

std::string IOT_RegDevice::GetDescription() const
{
    return m_description;
}

bool IOT_RegDevice::SetAttributes(const std::map<std::string, std::string>& attributes)
{
    if( attributes.size() > IOTAPI::IOTAPI_MAX_NUM_OF_ATTRIBUTES) {
        return false;
    }

    std::map<std::string, std::string>::const_iterator it = attributes.begin();
    while(it != attributes.end())
    {
        if(it->first.length() > IOTAPI::IOTAPI_MAX_ATTRIBUTE_LEN ||
           it->second.length() > IOTAPI::IOTAPI_MAX_ATTRIBUTE_LEN ||
           it->first.empty() || it->second.empty())
        {
            return false;
        }
        ++it;
    }

    m_attributes.clear();
    it = attributes.begin();
    while(it != attributes.end())
    {
        m_attributes[it->first] = it->second;
        ++it;
    }

    return true;
}

void IOT_RegDevice::GetAttributes(std::map<std::string, std::string>& attr) const
{
    std::map<std::string, std::string>::const_iterator it = m_attributes.begin();
    attr.clear();

    while(it != m_attributes.end()) {
        attr[it->first] = it->second;
        ++it;
    }
}

bool IOT_RegDevice::AppendAttribute(const std::string& key, const std::string& value)
{
    if(key.length() > IOTAPI::IOTAPI_MAX_ATTRIBUTE_LEN || value.length() > IOTAPI::IOTAPI_MAX_ATTRIBUTE_LEN
       || key.empty() || value.empty() || m_attributes.size() == IOTAPI::IOTAPI_MAX_NUM_OF_ATTRIBUTES)
    {
        return false;
    }

    m_attributes[key] = value;
    return true;
}

std::string IOT_RegDevice::GetAttribute(std::string key) const
{
    std::map<std::string, std::string>::const_iterator it = m_attributes.find(key);
    if(it != m_attributes.end()) {
        return it->second;
    }

    return "";
}

bool IOT_RegDevice::ToJSON(std::string &json) const
{
    Json::Value device;
    Json::FastWriter fastWriter;

    if(!ToJSON(device)) {
        return false;
    }

    json = fastWriter.write(device);
    return true;
}

bool IOT_RegDevice::ToJSON(Json::Value &json) const
{
    if(m_name.empty() || m_manufacturer.empty()) {
        return false;
    }

    json["name"] = m_name;
    json["manufacturer"] = m_manufacturer;

    if(!m_type.empty()) {
        json["type"] = m_type;
    }

    if(!m_description.empty()) {
        json["description"] = m_description;
    }

    std::map<std::string, std::string>::const_iterator it = m_attributes.begin();
    while(it != m_attributes.end())
    {
        Json::Value attr;
        attr["key"] = it->first;
        attr["value"] = it->second;
        json["attributes"].append(attr);
        ++it;
    }

    return true;
}

bool IOT_RegDevice::FromJSON(const std::string& json)
{
    Json::Value answer;
    Json::Reader json_reader;

    if(json_reader.parse(json, answer, false))
    {
        if(IOT_RegDevice::FromJSON(answer)) {
            return true;
        }
    }

    return false;
}

bool IOT_RegDevice::FromJSON(const Json::Value &json)
{
    IOT_RegDevice old(*this);
    m_type = "";
    m_description = "";
    m_attributes.clear();

    if(json.isMember("name") && json["name"].isString()) {
        if(!SetName(json["name"].asString())) {
            return false;
        }
    } else {
        return false;
    }

    if(json.isMember("manufacturer") && json["manufacturer"].isString()) {
        if(!SetManufacturer(json["manufacturer"].asString())) {
            IOT_RegDevice(old);
            return false;
        }
    } else {
        IOT_RegDevice(old);
        return false;
    }

    if(json.isMember("type") && json["type"].isString()) {
        if(!SetType( json["type"].asString() )) {
            IOT_RegDevice(old);
            return false;
        }
    }

    if(json.isMember("description") && json["description"].isString()) {
        if(!SetDescription( json["description"].asString() ))
        {
            IOT_RegDevice(old);
            return false;
        }
    }

    if(json.isMember("attributes") && json["attributes"].isArray())
    {
        Json::Value::iterator it = json["attributes"].begin();
        while(it != json["attributes"].end())
        {
            if((*it).isMember("key") && (*it)["key"].isString()
                && (*it).isMember("value") && (*it)["value"].isString())
            {
                if(!AppendAttribute((*it)["key"].asString(), (*it)["value"].asString()))
                {
                    IOT_RegDevice(old);
                    return false;
                }
            }
            ++it;
        }
    }

    return true;
}
