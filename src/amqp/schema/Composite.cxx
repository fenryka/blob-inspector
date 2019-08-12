#include "Composite.h"

#include "colours.h"

#include "amqp/schema/restricted-types/Restricted.h"

#include <iomanip>
#include <iostream>

/******************************************************************************/

namespace amqp::internal::schema {

    std::ostream &
    operator << (std::ostream & stream_, const Composite & clazz_) {
        stream_
            << "name       : " << clazz_.name() << std::endl
            << "label      : " << clazz_.m_label << std::endl
            << "descriptor : " << clazz_.descriptor() << std::endl
            << "fields     : ";

        for (auto const & i : clazz_.m_fields) stream_ << *i << std::setw (13) << " ";
        stream_ << std::setw(0);

        return stream_;
    }

}

/******************************************************************************
 *
 * amqp::internal::schema::Composite
 *
 ******************************************************************************/

amqp::internal::schema::
Composite::Composite (
        const std::string & name_,
        std::string label_,
        const std::list<std::string> & provides_,
        std::unique_ptr<Descriptor> & descriptor_,
        std::vector<std::unique_ptr<Field>> & fields_
) : AMQPTypeNotation (name_, descriptor_)
  , m_label (std::move (label_))
  , m_provides (provides_)
  , m_fields (std::move (fields_))
{ }

/******************************************************************************/

const std::vector<std::unique_ptr<amqp::internal::schema::Field>> &
amqp::internal::schema::
Composite::fields() const {
    return m_fields;
}

/******************************************************************************/

amqp::internal::schema::AMQPTypeNotation::Type
amqp::internal::schema::
Composite::type() const {
    return AMQPTypeNotation::Type::Composite;
}

/******************************************************************************/

/**
 * Use a visitor style pattern to work out weather two types, composite or
 * restricted, are "less than" one or not. In this case we define being
 * "less than" not having a type that the other depends on. This will
 * eventually give us a set ordered in such a way we can simply create
 * each element in turn
 *
 * "...This object determines the order of the elements in the container: it is
 * a function pointer or a function object that takes two arguments of the same
 * type as the container elements, and returns true if the first argument is
 * considered to go before the second in the strict weak ordering it defines,
 * and false otherwise. ..."

 *
 * @param rhs
 * @return
 */
bool
amqp::internal::schema::
Composite::dependsOn (const OrderedTypeNotation & rhs) const {
    std::cout << "composite " << BLUE << name() << RESET << " depended on RHS" << std::endl;
    return dynamic_cast<const AMQPTypeNotation &>(rhs).dependsOn(*this);
}

/******************************************************************************/

bool
amqp::internal::schema::
Composite::dependsOn (const amqp::internal::schema::Restricted & lhs_) const {
    std::cout << "composite " << BLUE << name() << RESET << " depended on by " << RED << lhs_.name() << RESET << std::endl;

    for (auto i { lhs_.begin() } ; i != lhs_.end() ; ++i) {
        std::cout << "   " << *i << " == " << name() << std::endl;
        if (*i == name()) {
            std::cout << " TRUE " << std::endl;
            return true;
        }
    }

    return false;
}

/*********************************************************o*********************/

bool
amqp::internal::schema::
Composite::dependsOn (const amqp::internal::schema::Composite & lhs_) const {
//    std::cout << "composite " << name() << " depends on composite " << lhs_.name() << std::endl;

    for (auto const & field : lhs_) {
//        std::cout << "  " << field->type() << " " << field->name() << std::endl;
        if (field->type() == name()) return true;
    }

    return false;
}

/******************************************************************************/
