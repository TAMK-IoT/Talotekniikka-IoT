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

#ifndef IOTAPI_DEFINES_H
#define IOTAPI_DEFINES_H

#include <string>
#include <stdint.h>

namespace IOTAPI
{
    const uint32_t IOTAPI_MAX_NAME_LEN = 100;
    const uint32_t IOTAPI_MAX_DESCRIPTION_LEN = 255;
    const uint32_t IOTAPI_MAX_ATTRIBUTE_LEN = 255;
    const uint32_t IOTAPI_MAX_NUM_OF_ATTRIBUTES = 50;
    const uint32_t IOTAPI_MAX_PATH_LEN = 1000;
    const uint32_t IOTAPI_MAX_UNIT_LEN = 10;
    const uint32_t IOTAPI_MAX_PATH_DEPTH = 10;

    const uint32_t IOTAPI_MAX_HREF_LEN = 200;
    const uint32_t IOTAPI_MAX_CREATED_LEN = 40;
    const uint32_t IOTAPI_DEVID_LEN = 32;

    //! Data types that are supported by IoT-Ticket API
    typedef enum
    {
        IOT_no_type = 0,
        IOT_double  = 1,
        IOT_long    = 2,
        IOT_string  = 3,
        IOT_bool    = 4,
        IOT_binary  = 5
    } IOT_DataType;

    //! Error codes for API client operations
    typedef enum
    {
        IOT_ERR_OK            = 0,  //! Success
        IOT_ERR_REGISTER_FAIL = 1,  //! Already registered
        IOT_ERR_AGAIN         = 2,  //! Call again
        IOT_ERR_PARAM         = 3,  //! Error in parameters
        IOT_ERR_INITIALIZED   = 4,  //! Library not initialized
        IOT_ERR_AUTH          = 5,  //! Username/pw combination incorrent
        IOT_ERR_ACCESS        = 6,  //! Username/pw combination doesn't have rights to do the operation
        IOT_ERR_QUOTA         = 7,  //! Quota ran out from the server
        IOT_ERR_SERVER        = 8,  //! Internal server error
        IOT_ERR_WRITE_FAILED  = 9,  //! Writing data failed
        IOT_ERR_CONN          = 10, //! Could not connect to WRM server because of network
        IOT_ERR_SSL           = 11, //! Could not connect to WRM server because SSL failed
        IOT_ERR_CURL_CALL     = 12, //! Other curl library related errors
        IOT_ERR_GENERAL       = 13  //! Error that could not be identified as none of the above
    } IOTAPI_err;

    //! Ordering of results for read process data queries
    typedef enum
    {
        IOT_ORDER_DEFAULT,
        IOT_ORDER_ASCENDING,
        IOT_ORDER_DESCENDING
    } IOT_DataOrder;
}

#endif // IOTAPI_DEFINES_H
