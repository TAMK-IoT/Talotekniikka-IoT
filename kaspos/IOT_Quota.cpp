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


#include "IOT_Quota.h"

IOT_Quota::IOT_Quota(): m_totalDevices(0), m_maxDevicesAllowed(0), m_maxNodesPerDevice(0),
    m_usedStorage(0), m_maxStorage(0)
{
}

int IOT_Quota::GetTotalDevices() const
{
    return m_totalDevices;
}

int IOT_Quota::GetMaxDevicesAllowed() const
{
    return m_maxDevicesAllowed;
}

int IOT_Quota::GetMaxNodesPerDevice() const
{
    return m_maxNodesPerDevice;
}

int IOT_Quota::GetUsedStorage() const
{
    return m_usedStorage;
}

int IOT_Quota::GetMaxStorage() const
{
    return m_maxStorage;
}

bool IOT_Quota::FromJSON(const std::string& json)
{
    Json::Value answer;
    Json::Reader json_reader;

    if(json_reader.parse(json, answer, false))
    {
        return FromJSON(answer);
    }

    return false;
}

bool IOT_Quota::FromJSON(const Json::Value& json)
{
    try {
        m_totalDevices = json["totalDevices"].asInt();
        m_maxDevicesAllowed = json["maxNumberOfDevices"].asInt();
        m_maxNodesPerDevice = json["maxDataNodePerDevice"].asInt();
        m_usedStorage = json["usedStorageSize"].asInt64();
        m_maxStorage = json["maxStorageSize"].asInt64();
    }
    catch(...) {
        return false;
    }

    return true;
}


