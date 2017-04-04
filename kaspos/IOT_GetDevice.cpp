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

#include "IOT_GetDevice.h"
#include "IOT_defines.h"
#include <stdio.h>

IOT_GetDevice::IOT_GetDevice(): IOT_RegDevice(), m_deviceID(""), m_href(""), m_createdAt("") {}

IOT_GetDevice::~IOT_GetDevice() {}

bool IOT_GetDevice::SetDeviceID(const std::string& devID)
{
    if(devID.length() != IOTAPI::IOTAPI_DEVID_LEN) {
        return false;
    }

    m_deviceID = devID;
    return true;
}

std::string IOT_GetDevice::GetDeviceID() const
{
    return m_deviceID;
}

bool IOT_GetDevice::SetHref(const std::string& href)
{
    if(href.empty() || href.length() > IOTAPI::IOTAPI_MAX_HREF_LEN) {
        return false;
    }

    m_href = href;
    return true;
}

std::string IOT_GetDevice::GetHref() const
{
    return m_href;
}

bool IOT_GetDevice::SetCreated(const std::string& created)
{
    if(created.empty() || created.length() > IOTAPI::IOTAPI_MAX_CREATED_LEN) {
        return false;
    }

    m_createdAt = created;
    return true;
}

std::string IOT_GetDevice::GetCreated() const
{
    return m_createdAt;
}

bool IOT_GetDevice::ToJSON(std::string &json) const
{
    Json::Value device;
    Json::FastWriter fastWriter;

    if(!IOT_GetDevice::ToJSON(device)) {
        return false;
    }

    json = fastWriter.write(device);
    return true;
}

bool IOT_GetDevice::ToJSON(Json::Value &json) const
{
    if(m_deviceID.empty() || m_href.empty() || m_createdAt.empty()) {
        return false;
    }

    json["deviceId"] = m_deviceID;
    json["href"] = m_href;
    json["createdAt"] = m_createdAt;

    if(!IOT_RegDevice::ToJSON(json)) {
        json.clear();
        return false;
    }

    return true;
}

bool IOT_GetDevice::FromJSON(const std::string &json)
{
    Json::Value answer;
    Json::Reader json_reader;

    if(json_reader.parse(json, answer, false))
    {
        if(IOT_GetDevice::FromJSON(answer)) {
            return true;
        }
    }

    return false;
}

bool IOT_GetDevice::FromJSON(const Json::Value &json)
{
    IOT_GetDevice old(*this);

    if(json.isMember("deviceId") && json["deviceId"].isString())
    {
        if(!SetDeviceID(json["deviceId"].asString())) {
            return false;
        }
    } else {
        return false;
    }

    if(json.isMember("href") && json["href"].isString())
    {
        if(!SetHref(json["href"].asString())) {
            IOT_GetDevice(old);
            return false;
        }
    } else {
        IOT_GetDevice(old);
        return false;
    }

    if(json.isMember("createdAt") && json["createdAt"].isString())
    {
        if(!SetCreated(json["createdAt"].asString())) {
            IOT_GetDevice(old);
            return false;
        }
    } else {
        IOT_GetDevice(old);
        return false;
    }

    if(!IOT_RegDevice::FromJSON(json)) {
        IOT_GetDevice(old);
        return false;
    }

    return true;
}
