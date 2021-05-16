#include "Map.h"
#include "List.h"
#include "Enum.h"
#include "SchemaUtils.h"

#include "described-types/Composite.h"

/******************************************************************************
 *
 * Map Member Functions
 *
 ******************************************************************************/

amqp::internal::schema::
Map::Map (
    uPtr<Descriptor> descriptor_,
    std::string name_,
    std::string label_,
    std::vector<std::string> provides_,
    std::string source_
) : Restricted (
        std::move (descriptor_),
        std::move (name_),
        std::move (label_),
        std::move (provides_),
        amqp::internal::schema::Restricted::RestrictedTypes::map_t
    ) , m_source { std::move (source_) }
{
    auto [map, of, to] = types::mapType (name());
    m_mapOf = { of, to };
}

/******************************************************************************/

std::vector<std::string>::const_iterator
amqp::internal::schema::
Map::begin() const {
    return m_mapOf.begin();
}

/******************************************************************************/

std::vector<std::string>::const_iterator
amqp::internal::schema::
Map::end() const  {
    return m_mapOf.end();
}

/******************************************************************************/

std::pair<
    std::reference_wrapper<const std::string>,
    std::reference_wrapper<const std::string>>
amqp::internal::schema::
Map::mapOf() const {
    return std::pair { std::cref (m_mapOf[0]), std::cref (m_mapOf[1]) };
}

/******************************************************************************/

int
amqp::internal::schema::
Map::dependsOnMap (const amqp::internal::schema::Map & lhs_) const {
    // do we depend on the lhs
    if (m_mapOf[0] == lhs_.name() || m_mapOf[1] == lhs_.name()) {
        return 1;
    }

    // does lhs_ depend on us
    auto lhsMapOf { lhs_.mapOf() };
    if (lhsMapOf.first.get() == name() || lhsMapOf.second.get() == name()) {
        return 2;
    }

    return 0;
}

/******************************************************************************/

int
amqp::internal::schema::
Map::dependsOnList (const amqp::internal::schema::List & lhs_) const {
    DBG ("Map::dependsOnList; listOf " << lhs_.name() << "(" << lhs_.listOf() << ")" << std::endl);
    DBG ("MapName - " << name() << std::endl);
    DBG ("MapOf - " << m_mapOf[0] << " - " << m_mapOf[1] << std::endl);
    // do we depend on the lhs
    if (m_mapOf[0] == lhs_.name() || m_mapOf[1] == lhs_.name()) {
        DBG ("Map::dependsOnList - 1" << std::endl);
        return 1;
    }

    // does lhs_ depend on us
    if (lhs_.listOf() == name()) {
        DBG ("Map::dependsOnList - 2" << std::endl);
        return 2;
    }

    DBG ("Map::dependsOnList - 0" << std::endl);
    return 0;
}

/******************************************************************************/

int
amqp::internal::schema::
Map::dependsOnArray (const amqp::internal::schema::Array & lhs_) const {
    return 0;
}

/******************************************************************************/

int
amqp::internal::schema::
Map::dependsOnEnum (const amqp::internal::schema::Enum & lhs_) const {
    // do we depend on the lhs
    if (m_mapOf[0] == lhs_.name() || m_mapOf[1] == lhs_.name()) {
        return 1;
    }

    // does lhs_ depend on us
    if (lhs_.name() == name()) {
        return 2;
    }

    return 0;
}

/******************************************************************************/

int
amqp::internal::schema::
Map::dependsOnRHS (const amqp::internal::schema::Composite & lhs_) const  {
    // do we depend on the lhs
    if (m_mapOf[0] == lhs_.name() || m_mapOf[1] == lhs_.name()) {
        return 1;
    }

    // does lhs_ depend on us
    for (const auto & field : lhs_.fields()) {
        if (field->resolvedType() == name()) {
            return 2;
        }
    }

    return 0;
}

/******************************************************************************/
