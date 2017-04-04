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

#ifndef IOT_API_H
#define IOT_API_H

#include <string>
#include <stdint.h>
#include <vector>
#include "IOT_defines.h"
#include "IOT_WriteData.h"
#include "IOT_ReadData.h"
#include "IOT_ReadDataFilter.h"
#include "IOT_RegDevice.h"
#include "IOT_GetDevice.h"
#include "IOT_RestClient.h"
#include "IOT_Quota.h"
#include "IOT_QuotaDevice.h"

//! \brief Main interface of the IoT-Ticket C++ Client
//! \note This class does not implement mutual exclusion for its operations.
//!       If multithreaded access is required, each thread should create its
//!       own instance from IOT_API.
class IOT_API
{
public:
    //! \brief Constructor for IoT library
    //! \param [in] serverAddress - IoT-Ticket server address to be used
    //! \param [in] auth          - Username that is used for authentication when communicating with IoT-Ticket server
    //! \param [in] password      - Password that is used for authentication when communicating with IoT-Ticket server
    //! \param [in] timeout       - Timeout of single operation when communicating with the server
    IOT_API(std::string serverAddress, std::string authName, std::string password, size_t timeout_s = 20);

    //! \brief Returns devices from IoT-Ticket server
    //! \param [out] devices - Devices returned from IoT-Ticket server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err GetDevices(std::vector<IOT_GetDevice>& devices) const;

    //! \brief Returns information of single device from server
    //! \param [in] devId   - Device ID for which the information is queried
    //! \param [out] device - Device information returned from IoT-Ticket server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err GetDevice(const std::string& devId, IOT_GetDevice& device) const;

    //! \brief Register device to the IoT-Ticket system
    //! \param [in] device - Information of the new device that is registered
    //! \param [out] devID - Device ID that IoT Ticker server has returned. This ID is used in further communication with IoT server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err RegisterDevice(const IOT_RegDevice &device, std::string& devID) const;

    //! \brief Send measurement data to the IoT-Ticket server
    //! \param [in] devId - Device ID one wants to write to
    //! \param [in] data  - Vector that contains the data to be written
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err SendData(const std::string& devId, const std::vector<IOT_WriteData>& data) const;

    //! \brief Send single measurement to the IoT-Ticket server
    //! \param [in] devId - Device ID one wants to write to
    //! \param [in] data  - Data to be written
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err SendData(const std::string& devId, const IOT_WriteData& data) const;

    //! \brief Read process data from the IoT-Ticket server
    //! \param [in] devId  - Device ID for the device from which data is read
    //! \param [in] filter - Filtering options for read query
    //! \param [out] data  - Process data returned by server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err ReadData(const std::string& devId, const IOT_ReadDataFilter& filter,
                                std::vector<IOT_ReadData>& data) const;

    //! \brief Get available datanodes for device (without process data)
    //! \param [in] devId - Device ID one wants to write to
    //! \param [out] data - Datanode list returned by server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err GetDatanodes(const std::string& devId, std::vector<IOT_ReadData>& data) const;

    //! \brief Get overall quota information
    //! \param [out] quota  - Quota information returned by server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err GetQuota(IOT_Quota& quota) const;

    //! \brief Get device specific quota information
    //! \param [in] devId   - Device for which the quota is queried
    //! \param [out] quota  - Quota information returned by server
    //! \return IOTAPI::IOT_ERR_OK if successful, error code otherwise
    IOTAPI::IOTAPI_err GetQuota(const std::string& devId, IOT_QuotaDevice& quota) const;

private:
    //! Removes last forward slash from URL string if present
    void RemoveTrailingSlash(std::string& str) const;

    //! Try to parse string to JSON object
    bool ParseJson(const std::string& str, Json::Value& value) const;

    //! Extract IoT-Ticket error code from server JSON reply
    IOTAPI::IOTAPI_err GetErrorCode(Json::Value& value) const;

    //! Base address of the IoT-Ticket server API
    std::string m_servAddr;

    //! Username for HTTP basic auth
    std::string m_authName;

    //! Password for HTTP basic auth
    std::string m_password;

    //! Client component that handles the HTTPS communication with server
    IOT_RestClient m_client;
};

#endif //IOT_API_H
