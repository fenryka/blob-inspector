#pragma once

#include "Field.h"

/******************************************************************************/

namespace amqp::internal::schema {

    class CompositeField final : public Field {
        private :
            static const std::string m_fieldType;

        public :
            CompositeField (
                std::string, std::string, std::list<std::string>,
                std::string, std::string, bool, bool);

            [[nodiscard]] bool primitive() const override;
            [[nodiscard]] const std::string & fieldType() const override;
            [[nodiscard]] const std::string & resolvedType() const override;
    };

}

/******************************************************************************/
