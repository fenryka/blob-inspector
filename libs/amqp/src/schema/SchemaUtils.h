#pragma once

#include <string>

/******************************************************************************/

namespace amqp::internal::schema::types {
    std::string unbox (const std::string &);

    std::pair<std::string, std::string> listType (const std::string &);
    std::tuple<std::string, std::string, std::string> mapType (const std::string &);
    void nestedGenericTypes (const std::string &, std::vector<std::string> &);

    bool isContainer (const std::string &);
    bool isArray (const std::string &);
    bool isList (const std::string &);
    bool isMap (const std::string &);
}

/******************************************************************************/
