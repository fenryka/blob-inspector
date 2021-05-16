#include <gtest/gtest.h>
#include <memory>

#include "src/schema/SchemaUtils.h"

/******************************************************************************/

using namespace amqp::internal::schema;

/******************************************************************************/

TEST (schema_utils_test, 1) { // NOLINT

    auto res = types::listType("a<b>");

    ASSERT_EQ("a", res.first);
    ASSERT_EQ("b", res.second);
}

/******************************************************************************/

TEST (schema_utils_test, 2) { // NOLINT

    auto res = types::listType("a<b<c>>");

    ASSERT_EQ("a", res.first);
    ASSERT_EQ("b<c>", res.second);
}

/******************************************************************************/

TEST (schema_utils_test, 3) { // NOLINT

    auto res = types::listType("a");

    ASSERT_EQ("a", res.first);
    ASSERT_EQ("", res.second);
}

/******************************************************************************/

TEST (schema_utils_test, 4) { // NOLINT
    std::string type = "java.util.List<java.util.Map<c, d>>";

    std::vector<std::string> v;
    types::nestedGenericTypes (type, v);

    ASSERT_EQ (4, v.size());

    ASSERT_TRUE (std::find (v.begin(), v.end(), "c") != v.end());
    ASSERT_TRUE (std::find (v.begin(), v.end(), "d") != v.end());
    ASSERT_TRUE (std::find (v.begin(), v.end(), "java.util.Map<c, d>") != v.end());
    ASSERT_TRUE (std::find (v.begin(), v.end(),
        "java.util.List<java.util.Map<c, d>>") != v.end());
}

/******************************************************************************/
