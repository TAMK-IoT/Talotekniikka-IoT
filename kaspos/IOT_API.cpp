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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "IOT_API.h"
#include "IOT_defines.h"
#include "IOT_WriteData.h"
#include "json/json.h"

using namespace IOTAPI;

static std::string IOT_DEVICE_PATH = "/devices";
static std::string IOT_WRITE_PATH  = "/process/write";
static std::string IOT_READ_PATH   = "/process/read";
static std::string IOT_QUOTA_PATH  = "/quota";


IOT_API::IOT_API(std::string serverAddress, std::string authName, std::string password, size_t timeout_s):
    m_servAddr(serverAddress), m_authName(authName), m_password(password)
{
    RemoveTrailingSlash(m_servAddr);
    m_client.SetRequestTimeout(timeout_s);
}

void IOT_API::RemoveTrailingSlash(std::string& str) const
{
    if(str.length() > 0 && str.at( str.length()-1 ) == '/') {
        str.erase( str.length()-1 );
    }
}

IOTAPI_err IOT_API::GetDevices(std::vector<IOT_GetDevice>& devices) const
{
    devices.clear();

    std::string response;
    IOTAPI::IOTAPI_err ret = m_client.GetResource(m_servAddr + IOT_DEVICE_PATH, m_authName, m_password, response);

    Json::Value devicesAnswer;
    if(ParseJson(response, devicesAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(devicesAnswer);
        }
        else if(devicesAnswer.isMember("items") && devicesAnswer["items"].isArray())
        {
            ret = IOT_ERR_OK;
            Json::Value::iterator it = devicesAnswer["items"].begin();
            while(it != devicesAnswer["items"].end())
            {
                IOT_GetDevice newDevice;
                if(newDevice.FromJSON(*it)) {
                    devices.push_back(newDevice);
                }
                ++it;
            }
        }
    }

    return ret;
}

IOTAPI_err IOT_API::GetDevice(const std::string& devId, IOT_GetDevice& device) const
{
    std::string response;
    std::string url = m_servAddr + IOT_DEVICE_PATH + "/" + devId;
    IOTAPI::IOTAPI_err ret = m_client.GetResource(url, m_authName, m_password, response);

    Json::Value devicesAnswer;
    if(ParseJson(response, devicesAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(devicesAnswer);
        }
        else if(!device.FromJSON(devicesAnswer)) {
            ret = IOT_ERR_GENERAL;
        }
    }

    return ret;
}

IOTAPI::IOTAPI_err IOT_API::RegisterDevice(const IOT_RegDevice& device, std::string &devID) const
{
    devID = "";

    std::string devJson;
    IOTAPI::IOTAPI_err ret = IOT_ERR_PARAM;

    if(device.ToJSON(devJson))
    {
        std::string response;
        ret = m_client.PostAndReadResponse(m_servAddr + IOT_DEVICE_PATH, m_authName, m_password, devJson, response);

        Json::Value registeredDevice;
        if(ParseJson(response, registeredDevice)) {
            if(ret != IOTAPI::IOT_ERR_OK) {
                ret = GetErrorCode(registeredDevice);
            }
            else if(registeredDevice.isMember("deviceId") && registeredDevice["deviceId"].isString()) {
                devID = registeredDevice["deviceId"].asString();
                ret = IOT_ERR_OK;
            } else {
                ret = IOT_ERR_GENERAL;
            }
        }
    }

    return ret;
}

IOTAPI::IOTAPI_err IOT_API::SendData(const std::string& devId, const IOT_WriteData& data) const {
    std::vector<IOT_WriteData> writeVec;
    writeVec.push_back(data);
    return SendData(devId, writeVec);
}

IOTAPI_err IOT_API::ReadData(const std::string& devId, const IOT_ReadDataFilter& filter, std::vector<IOT_ReadData>& data) const
{
    std::string response;
    std::string url = m_servAddr + IOT_READ_PATH + "/" + devId + filter.BuildParameterString();
    IOTAPI::IOTAPI_err ret = m_client.GetResource(url, m_authName, m_password, response);

    Json::Value devicesAnswer;
    if(ParseJson(response, devicesAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(devicesAnswer);
        }

        else if(devicesAnswer.isMember("datanodeReads") && devicesAnswer["datanodeReads"].isArray())
        {
            ret = IOT_ERR_OK;
            Json::Value::iterator it = devicesAnswer["datanodeReads"].begin();
            while(it != devicesAnswer["datanodeReads"].end())
            {
                IOT_ReadData item;

                if(item.FromJSON(*it)) {
                    data.push_back(item);
                }
                else {
                    ret = IOTAPI::IOT_ERR_GENERAL;
                    break;
                }
                ++it;
            }
        }
    }

    return ret;
}

IOTAPI_err IOT_API::GetDatanodes(const std::string& devId, std::vector<IOT_ReadData>& data) const
{
    std::string response;
    std::string url = m_servAddr + IOT_DEVICE_PATH + "/" + devId + "/datanodes";
    IOTAPI::IOTAPI_err ret = m_client.GetResource(url, m_authName, m_password, response);

    Json::Value datanodesAnswer;
    if(ParseJson(response, datanodesAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(datanodesAnswer);
        }
        else if(datanodesAnswer.isMember("items") && datanodesAnswer["items"].isArray())
        {
            ret = IOT_ERR_OK;
            Json::Value::iterator it = datanodesAnswer["items"].begin();
            while(it != datanodesAnswer["items"].end())
            {
                IOT_ReadData item;

                if(item.FromJSON(*it)) {
                    data.push_back(item);
                }
                else {
                    ret = IOTAPI::IOT_ERR_GENERAL;
                    break;
                }
                ++it;
            }
        }
    }

    return ret;
}

IOTAPI::IOTAPI_err IOT_API::SendData(const std::string& devId, const std::vector<IOT_WriteData>& data) const
{
    IOTAPI::IOTAPI_err ret = IOT_ERR_PARAM;
    Json::Value writeData;

    if(data.empty()) {
        return ret;
    }

    for(uint32_t i = 0; i < data.size(); ++i)
    {
        Json::Value oneData;
        if(data.at(i).ToJSON(oneData)) {
            writeData.append(oneData);
        } else {
            return ret;
        }
    }

    std::string url = m_servAddr + IOT_WRITE_PATH + "/" + devId;
    std::string response;
    Json::FastWriter fastWriter;
    std::string json = fastWriter.write(writeData);

    ret = m_client.PostAndReadResponse(url, m_authName, m_password, json, response);

    Json::Value writeAnswer;
    if(ParseJson(response, writeAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(writeAnswer);
        } else if(writeAnswer.isMember("totalWritten") && writeAnswer["totalWritten"].isIntegral()) {
            if(writeAnswer["totalWritten"].asUInt() == data.size())
                ret = IOT_ERR_OK;
        } else {
            ret = IOT_ERR_GENERAL;
        }
    }

    return ret;
}

IOTAPI_err IOT_API::GetQuota(IOT_Quota& quota) const
{
    std::string response;
    IOTAPI::IOTAPI_err ret = m_client.GetResource(m_servAddr + IOT_QUOTA_PATH + "/all", m_authName, m_password, response);

    Json::Value quotaAnswer;
    if(ParseJson(response, quotaAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(quotaAnswer);
        } else if(!quota.FromJSON(quotaAnswer)) {
            ret = IOT_ERR_GENERAL;
        }
    }

    return ret;
}

IOTAPI_err IOT_API::GetQuota(const std::string& devId, IOT_QuotaDevice& quota) const
{
    std::string response;
    std::string url = m_servAddr + IOT_QUOTA_PATH + "/" + devId;
    IOTAPI::IOTAPI_err ret = m_client.GetResource(url, m_authName, m_password, response);

    Json::Value quotaAnswer;
    if(ParseJson(response, quotaAnswer)) {
        if(ret != IOTAPI::IOT_ERR_OK) {
            ret = GetErrorCode(quotaAnswer);
        } else if(!quota.FromJSON(quotaAnswer)) {
            ret = IOT_ERR_GENERAL;
        }
    }

    return ret;
}

bool IOT_API::ParseJson(const std::string& str, Json::Value& value) const
{
    Json::Reader json_reader;

    if(!json_reader.parse(str, value, false))
    {
        return false;
    }

    return true;
}

IOTAPI::IOTAPI_err IOT_API::GetErrorCode(Json::Value& value) const
{
    int code = 0;
    if(value.isMember("code") && value["code"].isIntegral()) {
        code = value["code"].asInt();
    }

    switch(code) {
    case 8000:
        return IOT_ERR_GENERAL;
    case 8001:
        return IOT_ERR_ACCESS;
    case 8002:
        return IOT_ERR_QUOTA;
    case 8003:
        return IOT_ERR_PARAM;
    case 8004:
        return IOT_ERR_WRITE_FAILED;
    default:
        return IOT_ERR_GENERAL;
    }
}


