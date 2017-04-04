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


#ifndef IOT_QUOTADEVICE_H
#define IOT_QUOTADEVICE_H

#include <string>
#include <json/json.h>

//! \brief Result type for device specific quota query
class IOT_QuotaDevice
{
public:
    IOT_QuotaDevice();

    int GetReadRequestsToday() const;
    int GetMaxRequestsPerDay() const;
    int GetDataNodesCount() const;
    long GetStorageSize() const;
    std::string GetDeviceId() const;

    bool FromJSON(const std::string& json);
    bool FromJSON(const Json::Value& json);

private:
    int m_requestsToday;
    int m_maxRequestsPerDay;
    int m_dataNodes;
    long m_storageSize;
    std::string m_devId;

};

#endif // IOT_QUOTADEVICE_H
