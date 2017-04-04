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


#include "IOT_QuotaDevice.h"


IOT_QuotaDevice::IOT_QuotaDevice(): m_requestsToday(0), m_maxRequestsPerDay(0),
    m_dataNodes(0), m_storageSize(0)
{
}

int IOT_QuotaDevice::GetReadRequestsToday() const
{
    return m_requestsToday;
}

int IOT_QuotaDevice::GetMaxRequestsPerDay() const
{
    return m_maxRequestsPerDay;
}

int IOT_QuotaDevice::GetDataNodesCount() const
{
    return m_dataNodes;
}

long IOT_QuotaDevice::GetStorageSize() const
{
    return m_storageSize;
}

std::string IOT_QuotaDevice::GetDeviceId() const
{
    return m_devId;
}

bool IOT_QuotaDevice::FromJSON(const std::string& json)
{
    Json::Value answer;
    Json::Reader json_reader;

    if(json_reader.parse(json, answer, false))
    {
        return FromJSON(answer);
    }

    return false;
}

bool IOT_QuotaDevice::FromJSON(const Json::Value& json)
{
    try {
        m_requestsToday = json["totalRequestToday"].asInt();
        m_maxRequestsPerDay = json["maxReadRequestPerDay"].asInt();
        m_dataNodes = json["numberOfDataNodes"].asInt();
        m_storageSize = json["storageSize"].asUInt64();
        m_devId = json["deviceId"].asString();
    }
    catch(...) {
        return false;
    }

    return true;
}
