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

#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>
#include <stdint.h>

//! \brief Utility class to perform Base64 conversions
class IOT_Base64
{
public:
    //! \brief Encode binary data to Base64 string
    //! \param [in] input - Binary data to encode
    //! \param [in] len   - Length of the binary data
    //! \return input in Base64 formatted string
    static std::string encode(const uint8_t* input, uint32_t len);

    //! \brief Decode Base64 string to binary
    //! \param [in] input         - Base64 string to decode
    //! \param [out] decodedBytes - Decode output
    //! \return true if the input was valid Base64 and decode was successful
    static bool decode(const std::string& input, std::vector<uint8_t>& decodedBytes);
};

#endif // BASE64_H
