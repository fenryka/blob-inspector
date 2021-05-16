#include "SchemaUtils.h"

#include <map>
#include <vector>

/******************************************************************************/

namespace {

    /**
     * Map Java boxed type names to the corresponding primitive
     * C++ types
     */
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

    /**
     * The Corda serializer distinguishes between arrays of
     * primitives and arrays of boxed types with the following
     * markers.
     *
     * This isn't a concern in C++ given we don't have boxed types.
     */
    const std::string array { "[]" }; // NOLINT
    const std::string primArray { "[p]" }; // NOLINT

}

/******************************************************************************/

/**
 * Java has two types of primitive, boxed and unboxed, essentially actual
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

std::tuple<std::string, std::string, std::string>
amqp::internal::schema::types::
mapType (const std::string & map_) {
    auto pos = map_.find ('<');

    auto idx { pos + 1 };
    for (auto nesting { 0 } ; idx < map_.size(); ++idx) {
        if (map_[idx] == '<') {
            ++nesting;
        } else if (map_[idx] == '>') {
            --nesting;
        } else if (map_[idx] == ',' && nesting == 0) {
            break;
        }
    }

    auto map = std::string { map_.substr (0, pos) };
    auto of  = std::string { map_.substr (pos + 1, idx - pos - 1) };
    of = of.erase(0, of.find_first_not_of(' '));
    auto to  = std::string { map_.substr (idx + 1, map_.size() - (idx + 2)) };
    to = to.erase(0, to.find_first_not_of(' '));

    return { map, types::unbox (of), types::unbox (to) };
}

/******************************************************************************/

void
amqp::internal::schema::types::
nestedGenericTypes (const std::string & type_, std::vector<std::string> & v_) {
    v_.push_back (type_);

    if (isList (type_)) {
        auto v = listType (type_);

        nestedGenericTypes (v.second, v_);
    } else if (isMap (type_)) {
        auto v = mapType (type_);

        nestedGenericTypes (std::get<1> (v), v_);
        nestedGenericTypes (std::get<2> (v), v_);
    }
}

/******************************************************************************/

bool
amqp::internal::schema::types::
isContainer (const std::string & type_) {
    // when C++20 is done we can use .endswith, until then we have to do a reverse search
    return (   isArray (type_)
            || isList (type_)
            || isMap (type_));
}

/******************************************************************************/

/**
 * Used to differentiate between lists and arrays in the underlying code
 * which the Corda serialisation scheme treats differently
 */
bool
amqp::internal::schema::types::
isArray (const std::string & type_) {
    // when C++20 is done we can use .endswith, until then we have to do a reverse search
    return (   std::equal (type_.rbegin(), type_.rbegin() + array.size(), array.rbegin(), array.rend())
            || std::equal (type_.rbegin(), type_.rbegin() + primArray.size(), primArray.rbegin(), primArray.rend()));
}

/******************************************************************************/

bool
amqp::internal::schema::types::
isList (const std::string & type_) {
    return type_.find ("java.util.List") == 0;
}

/******************************************************************************/

bool
amqp::internal::schema::types::
isMap (const std::string & type_) {
    return type_.find ("java.util.Map") == 0;
}

/******************************************************************************/
