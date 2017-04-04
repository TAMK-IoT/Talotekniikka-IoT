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

#ifndef IOT_GETDEVICE_H
#define IOT_GETDEVICE_H

#include "IOT_RegDevice.h"

//! \brief Result type for Get Devices API query
class IOT_GetDevice: public IOT_RegDevice
{
public:
    IOT_GetDevice();
    virtual ~IOT_GetDevice();

    std::string GetDeviceID() const;
    std::string GetHref() const;
    std::string GetCreated() const;

    virtual bool ToJSON(std::string& json) const;
    virtual bool ToJSON(Json::Value& json) const;

    virtual bool FromJSON(const std::string& json);
    virtual bool FromJSON(const Json::Value& json);

private:
    bool SetDeviceID(const std::string& devID);
    bool SetHref(const std::string& href);
    bool SetCreated(const std::string& created);

    std::string m_deviceID;
    std::string m_href;
    std::string m_createdAt;
};

#endif // IOT_GETDEVICE_H
