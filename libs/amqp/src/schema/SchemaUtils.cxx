#include "SchemaUtils.h"

#include <map>
#include <string>

/******************************************************************************/

namespace {

    std::map<std::string, std::string> boxedToUnboxed = { // NOLINT
            { "java.lang.Integer", "int" },
            { "java.lang.Boolean", "bool" },
            { "java.lang.Byte", "char" },
            { "java.lang.Short", "short" },
            { "java.lang.Character", "char" },
            { "java.lang.Float", "float" },
            { "java.lang.Long", "long" },
            { "java.lang.Double", "double" }
    };

    const std::string array { "[]" }; // NOLINT
    const std::string primArray { "[p]" }; // NOLINT

}

/******************************************************************************/

/**
 * Java gas two types of primitive, boxed and unboxed, essentially actual
 * primitives and classes representing those primitives. Of course, we
 * don't care about that, so treat boxed primitives as their underlying
 * type.
 */
std::string
amqp::internal::schema::types::
unbox (const std::string & type_) {
    auto it = boxedToUnboxed.find (type_);
    if (it == boxedToUnboxed.end()) return type_;
    else return it->second;

}

/******************************************************************************/

std::pair<std::string, std::string>
amqp::internal::schema::types::
listType (const std::string & list_) {
    auto pos = list_.find ('<');

    return std::make_pair (
            std::string { types::unbox (list_.substr (0, pos)) },
            std::string { types::unbox (list_.substr(pos + 1, list_.size() - pos - 2)) }
    );
}

/******************************************************************************/

bool
amqp::internal::schema::types::
isContainer (const std::string & type_) {
    // when C++20 is done we can use .endswith, until then we have to do a reverse search
    return (   std::equal (type_.rbegin(), type_.rbegin() + array.size(), array.rbegin(), array.rend())
            || std::equal (type_.rbegin(), type_.rbegin() + primArray.size(), primArray.rbegin(), primArray.rend())
            || type_.find ("java.util.List") == 0);
}

/******************************************************************************/

bool
amqp::internal::schema::types::
isArray (const std::string & type_) {
    // when C++20 is done we can use .endswith, until then we have to do a reverse search
    return (   std::equal (type_.rbegin(), type_.rbegin() + array.size(), array.rbegin(), array.rend())
            || std::equal (type_.rbegin(), type_.rbegin() + primArray.size(), primArray.rbegin(), primArray.rend()));
}

/******************************************************************************/
