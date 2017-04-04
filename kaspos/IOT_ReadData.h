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

#ifndef IOT_READDATA_H
#define IOT_READDATA_H

#include "IOT_defines.h"
#include <vector>
#include <string>
#include <json/json.h>

//! \brief Result type for process read query
//! \note Process data values are stored internally as strings (the way the server
//! returned them), and the conversion is done on demand by GetConvertedValue().
class IOT_ReadData
{
public:
    IOT_ReadData();
    ~IOT_ReadData();

    std::string GetName() const;
    std::string GetPath() const;
    std::string GetUnit() const;
    IOTAPI::IOT_DataType GetDatatype() const;

    //! \brief Get number of values returned by server
    //! \return Number of values available
    size_t ProcessValues();

    //! \brief Get timestamp for a value based on index
    //! \param [in] index - Index of process value timestamp [0...ProcessValues()-1]
    //! \param [out] ts   - Timestamp value
    //! \return true if index was valid and ts was set, false otherwise
    bool GetTimestamp(size_t index, unsigned long& ts) const;

    //! \brief Convert process data value to long
    //! \pre GetDatatype() == IOTAPI::IOT_long
    //! \param [in] index  - Index of process value [0...ProcessValues()-1]
    //! \param [out] value - Value as long
    //! \return true if index was valid and value was set successfully, false otherwise
    bool GetConvertedValue(size_t index, long& value) const;

    //! \brief Convert process data value to double
    //! \pre GetDatatype() == IOTAPI::IOT_double
    //! \param [in] index  - Index of process value [0...ProcessValues()-1]
    //! \param [out] value - Value as double
    //! \return true if index was valid and value was set successfully, false otherwise
    bool GetConvertedValue(size_t index, double& value) const;

    //! \brief Convert process data value to bool
    //! \pre GetDatatype() == IOTAPI::IOT_bool
    //! \param [in] index  - Index of process value [0...ProcessValues()-1]
    //! \param [out] value - Value as boolean
    //! \return true if index was valid and value was set successfully, false otherwise
    bool GetConvertedValue(size_t index, bool& value) const;

    //! \brief Return process data value as binary
    //! \pre GetDatatype() == IOTAPI::IOT_binary
    //! \param [in] index         - Index of process value [0...ProcessValues()-1]
    //! \param [out] decodedBytes - Value as vector of bytes
    //! \return true if index was valid and decodedBytes was set successfully, false otherwise
    bool GetConvertedValue(size_t index, std::vector<uint8_t>& decodedBytes) const;

    //! \brief Return process data value as string
    //! \note This function can be used even if the datatype is not string
    //! \param [in] index  - Index of process value [0...ProcessValues()-1]
    //! \param [out] value - Value as std::string
    //! \return true if index was valid and value was set successfully, false otherwise
    bool GetConvertedValue(size_t index, std::string& value) const;

    bool FromJSON(const Json::Value& json);

private:
    //! Convert string type name to IOT_DataType enumeration
    IOTAPI::IOT_DataType ConvertToDatatype(const std::string& type);

    std::string m_name;
    std::string m_path;
    std::string m_unit;
    IOTAPI::IOT_DataType m_dataType;

    typedef std::pair<unsigned long, std::string> process_value_t;

    //! Timestamp-value pairs (values stored internally as strings)
    std::vector<process_value_t> m_processData;
};

#endif // IOT_READDATA_H
