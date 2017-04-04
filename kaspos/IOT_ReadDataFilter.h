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


#ifndef IOT_READDATAFILTER_H
#define IOT_READDATAFILTER_H

#include "IOT_defines.h"
#include <vector>

//! \brief Type to describe filtering conditions for process data read queries
class IOT_ReadDataFilter
{
public:
    IOT_ReadDataFilter();

    //! \brief Add datanode to filter condition
    //! \note If multiple datanodes match the name, data from all of them is
    //!       returned (e.g. /Path1/NodeName and /Path2/NodeName)
    //! \param [in] name - Name of the datanode for read
    void AddDatanode(const std::string& name);

    //! \brief Add datanode to filter condition using exact path
    //! \param [in] name - Name of the datanode for read
    //! \param [in] path - Path of the datanode
    void AddDatanode(const std::string& name, const std::string path);

    //! \brief Set date from which data is returned
    //! \param [in] ts - Unix timestamp in milliseconds
    void SetFromDate(unsigned long ts);

    //! \brief Set date to which data is returned
    //! \param [in] ts - Unix timestamp in milliseconds
    void SetToDate(unsigned long ts);

    //! \brief Maximum number of values to return
    //! \param [in] limit - max number of process data values
    void SetLimit(unsigned limit);

    //! \brief Set data ordering, ascending/descending (based on timestamp)
    //! \param [in] order - ascending / descending order
    void SetDataOrder(IOTAPI::IOT_DataOrder order);

    //! \brief Clear all previously set filtering parameters
    void Clear();

    //! \brief Build parameter string for URL
    std::string BuildParameterString() const;

private:
    //! Removes last forward slash from URL string if present
    void RemoveTrailingSlash(std::string& str) const;

    std::vector<std::string> m_datanodes;
    unsigned long m_fromDate;
    bool m_fromDateSet;

    unsigned long m_toDate;
    bool m_toDateSet;

    unsigned m_limit;
    IOTAPI::IOT_DataOrder m_order;
};

#endif // IOT_READDATAFILTER_H
