#pragma once

#include <iosfwd>

#include "amqp/src/schema/AMQPTypeNotation.h"

/******************************************************************************/

namespace amqp::internal::schema {

    class Choice final : public amqp::schema::ISchemaElement {
        public :
            friend std::ostream & operator << (std::ostream &, const Choice &);

        private :
            std::string m_choice;

        public :
            Choice() = delete;

            explicit Choice (std::string);

            [[nodiscard]] const std::string & choice() const;
    };

}

/******************************************************************************/
