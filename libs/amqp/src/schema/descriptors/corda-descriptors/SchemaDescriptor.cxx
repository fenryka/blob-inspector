#include "SchemaDescriptor.h"

#include "corda-utils/include/types.h"
#include "corda-utils/include/debug.h"

#include "AMQPDescriptor.h"

#include "proton-wrapper/include/proton_wrapper.h"

#include "proton/codec.h"

#include "amqp/include/schema/AMQPDescribed.h"
#include "descriptors/AMQPDescriptors.h"
#include "described-types/Schema.h"
#include "OrderedTypeNotations.h"
#include "AMQPTypeNotation.h"

#include <sstream>

/******************************************************************************/

amqp::internal::schema::descriptors::
SchemaDescriptor::SchemaDescriptor (
    std::string symbol_,
    int val_
) : AMQPDescriptor (std::move (symbol_), val_) {
}

/******************************************************************************/

uPtr<amqp::AMQPDescribed>
amqp::internal::schema::descriptors::
SchemaDescriptor::build (pn_data_t * data_) const {
    DBG ("SCHEMA" << std::endl); // NOLINT

    validateAndNext(data_);

    schema::OrderedTypeNotations<schema::AMQPTypeNotation> schemas;

    /*
     * The Schema is stored as a list of lists of described objects
     */
    {
        proton::auto_list_enter ale (data_);

        for (int i { 1 } ; pn_data_next(data_) ; ++i) {
            DBG ("  " << i << "/" << ale.elements() << std::endl); // NOLINT
            proton::auto_list_enter ale2 (data_);
            while (pn_data_next(data_)) {
                schemas.insert (
                    descriptors::dispatchDescribed<schema::AMQPTypeNotation> (
                        data_));

                DBG("=======" << std::endl << schemas << "======" << std::endl);
            }
        }
    }

    return std::make_unique<schema::Schema> (std::move (schemas));
}

/******************************************************************************/

void
amqp::internal::schema::descriptors::
SchemaDescriptor::readRaw (
        pn_data_t * data_,
        std::stringstream & ss_,
        const AutoIndent & ai_
) const {
    proton::is_list (data_);

    {
        AutoIndent ai { ai_ };
        proton::auto_list_enter ale (data_);

        for (int i { 1 } ; pn_data_next (data_) ; ++i) {
            proton::is_list (data_);
            ss_ << ai << i << "/" << ale.elements() <<"]";

            AutoIndent ai2 { ai };

            proton::auto_list_enter ale2 (data_);
            ss_ << " list: entries: " << ale2.elements() << std::endl;

            for (int j { 1 } ; pn_data_next (data_) ; ++j) {
                ss_ << ai2 << i << ":" << j << "/" << ale2.elements()
                        << "] " << std::endl;

                AMQPDescriptorRegistory[pn_data_type(data_)]->readRaw (
                        data_, ss_,
                        AutoIndent { ai2 });
            }
        }
    }
}

/******************************************************************************/

void
amqp::internal::schema::descriptors::
SchemaDescriptor::readAvro (
        pn_data_t * data_,
        std::stringstream & ss_,
        const AutoIndent & ai_
) const {
    DBG ("readAvro::Schema" << std::endl);

    proton::is_list (data_);

    {
        // schema wrapper, this list should always be 1, also move onto the
        // first element
        proton::auto_list_enter ale (data_, true);
        assert (ale.elements() == 1);

        // contents of the scema should also be a list, this time one
        // for each schema elmenet

        {
            proton::auto_list_enter ale2(data_);

            for (int j{1}; pn_data_next(data_); ++j) {

                AMQPDescriptorRegistory[pn_data_type(data_)]->readAvro(
                        data_, ss_,
                        ai_);
            }
        }
    }
}

/******************************************************************************/
