#include "Restricted.h"

#include <string>
#include <vector>
#include <iostream>

/******************************************************************************/

std::ostream &
amqp::internal::schema::
operator << (
    std::ostream & stream_,
    const amqp::internal::schema::Restricted & clazz_)
{
    stream_
        << "name       : " << clazz_.name() << std::endl
        << "label      : " << clazz_.m_label << std::endl
        << "descriptor : " << clazz_.descriptor();

    return stream_;
}

/******************************************************************************
 *
 * amqp::internal::schema::Restricted
 *
 ******************************************************************************/

amqp::internal::schema::
Restricted::Restricted (
    std::unique_ptr<Descriptor> & descriptor_,
    const std::string & name_,
    const std::string & label_,
    const std::vector<std::string> & provides_,
    const std::string & source_
) : AMQPTypeNotation (name_, descriptor_)
  , m_label (label_)
  , m_provides (provides_)
  , m_source (source_)
{ }

/******************************************************************************/

amqp::internal::schema::AMQPTypeNotation::Type
amqp::internal::schema::
Restricted::type() const {
    return AMQPTypeNotation::Type::Restricted;
}

/******************************************************************************/


