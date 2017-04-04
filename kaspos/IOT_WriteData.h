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

#ifndef IOT_WRITEDATA_H
#define IOT_WRITEDATA_H

#include <string>
#include <stdint.h>
#include "IOT_defines.h"
#include <json/json.h>

//! \brief Representation of single measurement that can be logged
//! to server.
class IOT_WriteData
{
    public:
        IOT_WriteData();
        ~IOT_WriteData();

        bool SetName(const std::string& name);
        bool SetPath(const std::string& path);
        bool SetUnit(const std::string& unit);

        void SetValue(const std::string& value);
        void SetValue(const char* value);
        void SetValue(double value);
        void SetValue(int64_t value);
        void SetValue(bool value);
        void SetValue(uint8_t* value, uint32_t size);

        //! \brief Set time for the value. Unix timestamp in milliseconds
        void SetTimeMs(uint64_t timeMs);
        void SetTimeToNow();

        bool ToJSON(std::string& json) const;
        bool ToJSON(Json::Value& json) const;

        IOT_WriteData& operator= (const IOT_WriteData& other);
        IOT_WriteData(const IOT_WriteData& other);
        IOT_WriteData(IOT_WriteData& other);

    private:
        static const uint32_t MAX_STATIC_SIZE;

        //! Clear dynamically allocated memory
        inline void ClearDynamic();
        const uint8_t* getDataPointer() const;

        //! Assign data from other IOT_WriteData object. Handles the copying
        //! of dynamic data if necessary.
        IOT_WriteData& Assign(const IOT_WriteData& other);

        std::string m_name;
        std::string m_path;
        std::string m_unit;

        IOTAPI::IOT_DataType m_dataType;

        uint8_t m_value[8];
        uint8_t* m_dynValue;
        uint32_t m_valSize;

        uint64_t m_timeStampMs;
};

#endif // IOT_WRITEDATA_H
