#include "proton_wrapper.h"

#include <sstream>
#include <iomanip>
#include <iostream>

#include <proton/types.h>
#include <proton/codec.h>

/******************************************************************************/

std::ostream&
operator << (std::ostream& stream, pn_data_t * data_) {
    auto type = pn_data_type(data_);
    stream << std::setw (2) << type << " " <<  pn_type_name (type);

    switch (type) {
        case PN_ULONG :
            {
                stream << " " << pn_data_get_ulong (data_);
                break;
            }
        case PN_LIST :
            {
                stream << " #entries: " << pn_data_get_list (data_);
                break;
            }
        case PN_STRING :
            {
                auto str = pn_data_get_string (data_);

                stream << " " << std::string (str.start, str.size);
                break;
            }
        case PN_INT :
            {
                stream << " " << pn_data_get_int (data_);
                break;
            }
        case PN_BOOL :
            {
                stream << " " << (pn_data_get_bool (data_) ? "true" : "false");
                break;
            }
        case PN_SYMBOL :
            {
                stream << " " << pn_data_get_symbol (data_).size;
                stream << std::endl << "   -> ";
                auto v = pn_data_get_symbol (data_);
                for (int i (0) ; i < v.size ; ++i) {
                    stream << *(v.start + i) << " ";
                }
                break;
            }

        default : break;


    }
    return stream;
}

/******************************************************************************/

/**
 * pn_data_enter always places the current pointer before the first node. This
 * is a simple convienience function to avoid having to move to the first
 * element in addition to entering a child.
 */
bool
proton::pn_data_enter(pn_data_t * data_) {
    ::pn_data_enter(data_);
    return pn_data_next(data_);
}

/******************************************************************************/

void
proton::is_described (pn_data_t * data_) {
    if (pn_data_type(data_) != PN_DESCRIBED) {
        throw std::runtime_error ("Expected a described type");
    }
}

/******************************************************************************/

void
proton::is_ulong (pn_data_t * data_) {
    auto t = pn_data_type(data_);
    if (t != PN_ULONG) {
        std::stringstream ss;
        ss << "Expected an unsigned long but recieved " << pn_type_name (t);
        throw std::runtime_error (ss.str());
    }
}

/******************************************************************************/

inline void
proton::is_symbol (pn_data_t * data_) {
    if (pn_data_type(data_) != PN_SYMBOL) {
        throw std::runtime_error ("Expected an unsigned long");
    }
}

/******************************************************************************/

void
proton::is_list (pn_data_t * data_) {
    if (pn_data_type(data_) != PN_LIST) {
        throw std::runtime_error ("Expected a list");
    }
}

/******************************************************************************/

inline void
proton::is_string (pn_data_t * data_, bool allowNull) {
    if (pn_data_type(data_) != PN_STRING) {
        if (allowNull && pn_data_type(data_) != PN_NULL) {
            throw std::runtime_error ("Expected a String");
        }
    }
}

/******************************************************************************/

pn_bytes_t
proton::get_symbol (pn_data_t * data_) {
    is_symbol (data_);
    return pn_data_get_symbol(data_);
}

/******************************************************************************/

std::string
proton::get_string (pn_data_t * data_, bool allowNull) {
    if (pn_data_type(data_) == PN_STRING) {
        auto str = pn_data_get_string (data_);
        return std::string (str.start, str.size);
    } else  if (allowNull && pn_data_type(data_) == PN_NULL) {
        return "";
    }
    throw std::runtime_error ("Expected a String");
}

/******************************************************************************/

bool
proton::get_boolean (pn_data_t * data_) {
    if (pn_data_type(data_) == PN_BOOL) {
        return pn_data_get_bool (data_);
    }
    throw std::runtime_error ("Expected a boolean");
}

/******************************************************************************
 *
 * proton::auto_enter
 *
 ******************************************************************************/

proton::
auto_enter::auto_enter (pn_data_t * data_)
    : m_data (data_)
{
    proton::pn_data_enter(m_data);
}

/******************************************************************************/

proton::
auto_enter::~auto_enter() {
    pn_data_exit(m_data);
}

/******************************************************************************
 *
 * proton::auto_enter_and_next
 *
 ******************************************************************************/

proton::
auto_enter_and_next::auto_enter_and_next (pn_data_t * data_)
    : m_data (data_)
{
    proton::pn_data_enter(m_data);
}

/******************************************************************************/

proton::
auto_enter_and_next::~auto_enter_and_next() {
    pn_data_exit(m_data);
    pn_data_next(m_data);
}

/******************************************************************************
 *
 * proton::auto_list_enter
 *
 ******************************************************************************/

proton::
auto_list_enter::auto_list_enter (pn_data_t * data_)
    : m_elements (pn_data_get_list (data_))
    , m_data (data_)
{
   ::pn_data_enter(m_data);
}

/******************************************************************************/

proton::
auto_list_enter::~auto_list_enter() {
    pn_data_exit(m_data);
}

/******************************************************************************/

size_t
proton::
auto_list_enter::elements() const {
    return m_elements;
}

/******************************************************************************/

