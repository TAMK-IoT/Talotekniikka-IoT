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

#ifndef IOT_RESTCLIENT_H
#define IOT_RESTCLIENT_H

#include <curl/curl.h>
#include <string>
#include "IOT_defines.h"

//! \brief HTTP communication implemented using cUrl
class IOT_RestClient
{
public:

    IOT_RestClient();
    ~IOT_RestClient();

    //! \brief Set maximum response size that is accpeted from server
    //! \param [in] size - Maximum accpeted size in bytes
    void SetMaxResponseSize(size_t size);

    //! \brief Set timeout for connect and requests
    //! \param [in] timeout_s - Set timeout for connect and GET/POST operations
    void SetRequestTimeout(size_t timeout_s);

    //! \brief Perform a GET request to specific URL
    //! \param [in] url       - Target address
    //! \param [in] user      - Username for HTTP AUTH. Use empty string to disable AUTH
    //! \param [in] pw        - Password for HTTP AUTH
    //! \param [out] response - Response returned by remote server
    IOTAPI::IOTAPI_err GetResource(const std::string& url, const std::string& user,
                     const std::string& pw, std::string& response) const;

    //! \brief Perform a POST call to specific URL
    //! \param [in] url       - Target address
    //! \param [in] user      - Username for HTTP AUTH. Use empty string to disable AUTH
    //! \param [in] pw        - Password for HTTP AUTH
    //! \param [in] data      - POST payload which is sent to server
    //! \param [out] response - Response returned by remote server
    IOTAPI::IOTAPI_err PostAndReadResponse(const std::string& url, const std::string& user,
                             const std::string& pw, const std::string& data,
                             std::string& response) const;

private:
    //! Bookeeping structure for data sending in libcurl callback function
    struct WriteData
    {
        WriteData()
        {
            data = NULL;
            pos = 0;
        }
        const std::string* data;
        size_t pos;
    };

    //! Bookeeping structure for data receiving in libcurl callback function
    struct ReadData
    {
        ReadData()
        {
            data = NULL;
            maxSize = 0;
        }
        std::string* data;
        size_t maxSize;
    };

    static const size_t REST_DEFAULT_REQ_MAX_SIZE;
    static bool m_globalInit;

    //! libcurl callback to read server response
    static int ReadServerResponse(char *data, size_t size, size_t nmemb, void *buffer_in);

    //! libcurl callback to discard server response
    static int DiscardServerResponse(char *data, size_t size, size_t nmemb, void *buffer_in);

    //! libcurl callback to provide payload data
    static size_t WriteToServer(void *ptr, size_t size, size_t nmemb, void *userp);

    //! Set libcurl parameters based on query
    void CreateCurlCall(std::string url, bool postCall, std::string user, std::string pw) const;

    //! Perform the query previsouly prepared by CreateCurlCall
    CURLcode PerformCurlCall(ReadData* readPtr, WriteData* writePtr) const;

    //! Check if HTTP status code indicates success
    bool HttpStatusSuccess(long unsigned int status) const;

    //! Convert libcurl specific error code to IOT_API error code
    IOTAPI::IOTAPI_err GetReturnCode(long httpCode, CURLcode code) const;

    //! Max number of bytes allowed for server response
    size_t m_maxRequestSize;

    //! HTTP header fields added to queries
    curl_slist* m_headers;

    //! Handle to libcurl library
    CURL* m_curl;
};

#endif // IOT_RESTCLIENT_H
