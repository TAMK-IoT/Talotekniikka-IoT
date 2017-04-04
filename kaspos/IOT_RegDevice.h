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

#ifndef IOT_REGDEVICE_H
#define IOT_REGDEVICE_H

#include <string>
#include <map>
#include <json/json.h>

//! \brief Device information to register new device to server
class IOT_RegDevice
{
public:

    IOT_RegDevice();
    virtual ~IOT_RegDevice();

    //! \param [in] name - Short name of the device (Mandatory, max 100 characters)
    //! \return true if name was valid, false otherwise
    bool SetName(const std::string& name);
    std::string GetName() const;

    //! \param [in] manufacturer - Manufacturer of the device (Mandatory, max 100 characters)
    //! \return true if manufacturer was valid, false otherwise
    bool SetManufacturer(const std::string& manufacturer);
    std::string GetManufacturer() const;

    //! \param [in] type - Type of the device (Optional, max 100 characters)
    //! \return true if type was valid, false otherwise
    //! \note This field should describe the main category the device belongs.
    bool SetType(const std::string& type);
    std::string GetType() const;

    //! \param [in] desc - Description of the device (Optional, max 255 characters)
    //! \return true if type was valid, false otherwise
    bool SetDescription(const std::string& desc);
    std::string GetDescription() const;

    //! \param [in] attributes - Key-Value attribute pairs (Optional, max 50 attributes,
    //! key or attribute value cannot exceed 255 characters)
    //! \return true if attributes were valid, false otherwise
    bool SetAttributes(const std::map<std::string, std::string>& attributes);
    void GetAttributes(std::map<std::string, std::string>& attr) const;

    //! \param [in] key - Key for new attribute (max 255 characters)
    //! \param [in] value - Attribute value (max 255 characters)
    //! \return true if type was valid, false otherwise
    bool AppendAttribute(const std::string& key, const std::string& value);

    //! Get attribute value for specific key
    //! \return attribute value if found, empty string otherwise
    std::string GetAttribute(std::string key) const;

    virtual bool ToJSON(std::string& json) const;
    virtual bool ToJSON(Json::Value& json) const;

    virtual bool FromJSON(const std::string& json);
    virtual bool FromJSON(const Json::Value& json);

private:
    //! This short name for the device.
    std::string m_name;

    //! Short name for the device manufacturer
    std::string m_manufacturer;

    //! This field should describe the main category the device belongs.
    std::string m_type;

    //! A description of the device.
    std::string m_description;

    //! Contains map of key value pairs.
    std::map<std::string, std::string> m_attributes;
};

#endif // IOT_REGDEVICE_H
