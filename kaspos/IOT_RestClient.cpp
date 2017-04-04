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

#include "IOT_RestClient.h"

#include <string.h>
#include <algorithm>
#include <iostream>

const size_t IOT_RestClient::REST_DEFAULT_REQ_MAX_SIZE = 50000;

bool IOT_RestClient::m_globalInit = false;

static const long unsigned int HTTP_STATUS_OK           = 200;
static const long unsigned int HTTP_STATUS_CREATED      = 201;
static const long unsigned int HTTP_STATUS_ACCEPTED     = 202;
static const long unsigned int HTTP_STATUS_UNAUTHORIZED = 401;


int IOT_RestClient::ReadServerResponse(char* data, size_t size, size_t nmemb, void* buffer_in)
{
    size_t sizeToSave = size*nmemb;
    struct ReadData* dataPtr = (struct ReadData*)buffer_in;
    if(sizeToSave > 0)
    {
        if((dataPtr->data->size() + sizeToSave) >= dataPtr->maxSize)
        {
            return -1;
        } else {
            dataPtr->data->append(data, sizeToSave);
        }

        return sizeToSave;
    }

    return 0;
}


int IOT_RestClient::DiscardServerResponse(char* /*data*/, size_t size, size_t nmemb, void* /*buffer_in*/)
{
    return size*nmemb;
}


size_t IOT_RestClient::WriteToServer(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct WriteData *wdata = (struct WriteData *)userp;
    size_t sendBufSize = size*nmemb;
    if(sendBufSize < 1) {
        return 0;
    }

    size_t amount = std::min(wdata->data->size() - wdata->pos, sendBufSize);

    if(amount <= 0)
        return 0;

    char* dest = (char*)ptr;
    for(size_t i=0; i<amount; ++i)
    {
        *dest = wdata->data->at( wdata->pos );
        ++dest;
        wdata->pos += 1;
    }

    return amount;
}


IOT_RestClient::IOT_RestClient():
    m_maxRequestSize(REST_DEFAULT_REQ_MAX_SIZE)
{
    if(!m_globalInit) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        m_globalInit = true;
    }

    m_curl = curl_easy_init();
    m_headers = NULL;
    m_headers = curl_slist_append(m_headers, "Accept: application/json");
    m_headers = curl_slist_append(m_headers, "Content-Type: application/json");
    m_headers = curl_slist_append(m_headers, "Expect: ");
    m_headers = curl_slist_append(m_headers, "charsets: utf-8");

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
}


IOT_RestClient::~IOT_RestClient()
{
    curl_easy_cleanup(m_curl);
    m_curl = NULL;
}


void IOT_RestClient::SetMaxResponseSize(size_t size)
{
    m_maxRequestSize = size;
}

void IOT_RestClient::SetRequestTimeout(size_t timeout_s)
{
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, timeout_s);
    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, timeout_s);
}


IOTAPI::IOTAPI_err IOT_RestClient::GetResource(const std::string& url, const std::string& user, const std::string& pw, std::string& response) const
{
    response.clear();
    CreateCurlCall(url, false, user, pw);

    CURLcode res;
    ReadData rdata;
    rdata.data = &response;
    rdata.maxSize = m_maxRequestSize;
    long http_code = 0;

    res = PerformCurlCall(&rdata, NULL);
    curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &http_code);

    return GetReturnCode(http_code, res);
}


IOTAPI::IOTAPI_err IOT_RestClient::PostAndReadResponse(const std::string& url, const std::string& user, const std::string& pw,
                                     const std::string& data, std::string& response) const
{
    CreateCurlCall(url, true, user, pw);
    response.clear();

    CURLcode res;
    long http_code = 0;

    WriteData wdata;
    wdata.data = &data;
    wdata.pos = 0;

    ReadData rdata;
    rdata.data = &response;
    rdata.maxSize = m_maxRequestSize;

    res = PerformCurlCall(&rdata, &wdata);
    curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &http_code);

    return GetReturnCode(http_code, res);
}



void IOT_RestClient::CreateCurlCall(std::string url, bool postCall, std::string user, std::string pw) const
{
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    if(!user.empty()) {
        curl_easy_setopt(m_curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        std::string basicAuth = user + ":" + pw;
        curl_easy_setopt(m_curl, CURLOPT_USERPWD, basicAuth.c_str());
    }
    else {
        curl_easy_setopt(m_curl, CURLOPT_HTTPAUTH, 0);
    }

    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);

    if(postCall) {
        curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
    } else {
        curl_easy_setopt(m_curl, CURLOPT_POST, 0L);
    }

#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION

#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
}

CURLcode IOT_RestClient::PerformCurlCall(ReadData* readPtr, WriteData* writePtr) const
{
    if(writePtr != NULL) {
        curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, WriteToServer);
        curl_easy_setopt(m_curl, CURLOPT_READDATA, writePtr);
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, writePtr->data->size());
    }
    else {
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, 0);
        curl_easy_setopt(m_curl, CURLOPT_READDATA, NULL);
    }

    if(readPtr != NULL) {
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ReadServerResponse);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, readPtr);
    }
    else {
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, DiscardServerResponse);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, NULL);
    }

    return curl_easy_perform(m_curl);
}

bool IOT_RestClient::HttpStatusSuccess(unsigned long status) const
{
    return (status == HTTP_STATUS_OK) || (status == HTTP_STATUS_CREATED) ||
            (status == HTTP_STATUS_ACCEPTED);
}

IOTAPI::IOTAPI_err IOT_RestClient::GetReturnCode(long httpCode, CURLcode code) const
{
    if(HttpStatusSuccess(httpCode) && code == CURLE_OK)
        return IOTAPI::IOT_ERR_OK;

    if(httpCode == HTTP_STATUS_UNAUTHORIZED)
        return IOTAPI::IOT_ERR_AUTH;

    switch(code) {
    case CURLE_URL_MALFORMAT:
        return IOTAPI::IOT_ERR_PARAM;

    case CURLE_LOGIN_DENIED:
        return IOTAPI::IOT_ERR_AUTH;

    case CURLE_AGAIN:
        return IOTAPI::IOT_ERR_AGAIN;

    case CURLE_COULDNT_RESOLVE_HOST:
    case CURLE_COULDNT_CONNECT:
    case CURLE_OPERATION_TIMEDOUT:
        return IOTAPI::IOT_ERR_CONN;

    case CURLE_SSL_CONNECT_ERROR:
    case CURLE_SSL_ENGINE_NOTFOUND:
    case CURLE_SSL_ENGINE_SETFAILED:
    case CURLE_SSL_CERTPROBLEM:
    case CURLE_SSL_CIPHER:
    case CURLE_SSL_CACERT:
    case CURLE_USE_SSL_FAILED:
    case CURLE_SSL_ENGINE_INITFAILED:
    case CURLE_SSL_CACERT_BADFILE:
    case CURLE_SSL_SHUTDOWN_FAILED:
    case CURLE_SSL_CRL_BADFILE:
    case CURLE_SSL_ISSUER_ERROR:
        return IOTAPI::IOT_ERR_SSL;

    default:
        return IOTAPI::IOT_ERR_CURL_CALL;
    }
}

