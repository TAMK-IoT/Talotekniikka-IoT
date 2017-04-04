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
 *
 * Implementation of the Base64 encoding algorithm is based on Public Domain C++ example
 * algorithm available at:
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 */


#include "IOT_Base64.h"


const static std::string encodeLookup = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const static char padCharacter = '=';


std::string IOT_Base64::encode(const uint8_t* input, uint32_t len)
{
    std::string encodedString;

    encodedString.reserve(((len/3) + (len % 3 > 0)) * 4);

    unsigned int temp;
    size_t cursor = 0;
    for(size_t idx = 0; idx < len/3; idx++)
    {
        temp  = input[cursor++] << 16;
        temp += input[cursor++] << 8;
        temp += input[cursor++];

        encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
        encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
        encodedString.append(1, encodeLookup[(temp & 0x00000FC0) >> 6 ]);
        encodedString.append(1, encodeLookup[(temp & 0x0000003F)      ]);
    }

    if(len % 3 == 1) {
        temp  = input[cursor++] << 16;
        encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
        encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
        encodedString.append(2, padCharacter);
    }
    else if(len % 3 == 2) {
        temp  = input[cursor++] << 16;
        temp += input[cursor++] << 8;
        encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
        encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
        encodedString.append(1, encodeLookup[(temp & 0x00000FC0) >> 6 ]);
        encodedString.append(1, padCharacter);
    }

    return encodedString;
}

bool IOT_Base64::decode(const std::string& input, std::vector<uint8_t>& decodedBytes)
{
    if (input.length() % 4)
        return false;

    size_t padding = 0;
    if (input.length() >= 2)
    {
        if (input[input.length()-1] == padCharacter)
            padding++;
        if (input[input.length()-2] == padCharacter)
            padding++;
    }

    decodedBytes.clear();
    decodedBytes.reserve(((input.length()/4)*3) - padding);

    unsigned int temp = 0;
    std::string::const_iterator cursor = input.begin();
    while (cursor < input.end())
    {
        for (size_t quantumPosition = 0; quantumPosition < 4; quantumPosition++)
        {
            temp <<= 6;
            if(*cursor >= 0x41 && *cursor <= 0x5A)
                temp |= *cursor - 0x41;
            else if(*cursor >= 0x61 && *cursor <= 0x7A)
                temp |= *cursor - 0x47;
            else if(*cursor >= 0x30 && *cursor <= 0x39)
                temp |= *cursor + 0x04;
            else if(*cursor == 0x2B)
                temp |= 0x3E; //change to 0x2D for URL alphabet
            else if(*cursor == 0x2F)
                temp |= 0x3F; //change to 0x5F for URL alphabet
            else if(*cursor == padCharacter)
            {
                switch( input.end() - cursor )
                {
                case 1:
                    decodedBytes.push_back((temp >> 16) & 0x000000FF);
                    decodedBytes.push_back((temp >> 8 ) & 0x000000FF);
                    return true;
                case 2: //Two pad characters
                    decodedBytes.push_back((temp >> 10) & 0x000000FF);
                    return true;
                default:
                    return false;
                }
            }  else {
                return false;
            }
            cursor++;
        }
        decodedBytes.push_back((temp >> 16) & 0x000000FF);
        decodedBytes.push_back((temp >> 8 ) & 0x000000FF);
        decodedBytes.push_back((temp      ) & 0x000000FF);
    }

    return true;
}




