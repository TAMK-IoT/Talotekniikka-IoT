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

#include "IOT_ReadDataFilter.h"
#include <sstream>
#include <iostream>

IOT_ReadDataFilter::IOT_ReadDataFilter(): m_fromDate(0), m_fromDateSet(false), m_toDate(0),
    m_toDateSet(false), m_limit(0), m_order(IOTAPI::IOT_ORDER_DEFAULT)
{
}

void IOT_ReadDataFilter::AddDatanode(const std::string& name)
{
    m_datanodes.push_back(name);
}

void IOT_ReadDataFilter::AddDatanode(const std::string& name, const std::string path)
{
    std::string node = path;
    RemoveTrailingSlash(node);
    node += "/";
    node += name;
     m_datanodes.push_back(node);
}

void IOT_ReadDataFilter::SetFromDate(unsigned long ts)
{
    m_fromDate = ts;
    m_fromDateSet = true;
}

void IOT_ReadDataFilter::SetToDate(unsigned long ts)
{
    m_toDate = ts;
    m_toDateSet = true;
}

void IOT_ReadDataFilter::SetLimit(unsigned limit)
{
    m_limit = limit;
}

void IOT_ReadDataFilter::SetDataOrder(IOTAPI::IOT_DataOrder order)
{
    m_order = order;
}

void IOT_ReadDataFilter::Clear()
{
    m_fromDate = 0;
    m_fromDateSet = false;
    m_toDate = 0;
    m_toDateSet = false;
    m_limit = 0;
    m_datanodes.clear();
    m_order = IOTAPI::IOT_ORDER_DEFAULT;
}

std::string IOT_ReadDataFilter::BuildParameterString() const
{
    std::string params = "?datanodes=";

    std::vector<std::string>::const_iterator it = m_datanodes.begin();
    while(it != m_datanodes.end()) {

        if((it+1) != m_datanodes.end())
            params += ",";

        params += *it;
        ++it;
    }

    if(m_fromDateSet) {
        std::stringstream ss;
        ss << m_fromDate;
        params += "&fromdate=" + ss.str();
    }

    if(m_toDateSet) {
        std::stringstream ss;
        ss << m_toDate;
        params += "&todate=" + ss.str();
    }

    if(m_limit != 0) {
        std::stringstream ss;
        ss << m_limit;
        params += "&limit=" + ss.str();
    }

    if(m_order == IOTAPI::IOT_ORDER_ASCENDING)
        params += "&order=ascending";
    else if(m_order == IOTAPI::IOT_ORDER_DESCENDING)
        params += "&order=descending";

    return params;
}

void IOT_ReadDataFilter::RemoveTrailingSlash(std::string& str) const
{
    if(str.length() > 0 && str.at( str.length()-1 ) == '/') {
        str.erase( str.length()-1 );
    }
}
