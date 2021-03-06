#pragma once

/******************************************************************************/

#include <string>
#include "corda-utils/include/debug.h"

#include "schema/SchemaDumpTargets.h"

#include "amqp/src/serialiser/Serialiser.h"

/******************************************************************************/

struct pn_data_t;

namespace amqp {

    class CordaBytes;
    class CompositeFactory;

}

namespace amqp::serializable {

    class Serializable;
    class RestrictedSerializable;

}

/******************************************************************************/

namespace amqp {

    class AMQPBlob {
        private :
           mutable pn_data_t * m_data;

            template<class T, bool = std::is_pointer_v<T>>
            struct ReadPrimitive {
                static
                T
                read (const AMQPBlob & blob_) {
                    T v;
                    amqp::internal::serialiser::PrimToSerialiser<
                        std::remove_const_t<T>
                    >::get (&v, blob_.m_data);

                    return v;
                }
            };

            template<class T>
            struct ReadPrimitive<T, true> {
                static
                T
                read (const AMQPBlob & blob_) {
                    T v = new std::remove_pointer_t<T> {};
                    amqp::internal::serialiser::PrimToSerialiser<
                        std::remove_pointer_t<std::remove_const_t<T>>
                    >::get (v, blob_.m_data);

                    return v;
                }
            };

        protected :
            /*
             * Want [[ModifiableAMQPBlob]] to be able to construct
             * empty instances but in general don't want that functionality
             * exposed
             */
            AMQPBlob();

        public :
            explicit AMQPBlob (CordaBytes &);
            explicit AMQPBlob (pn_data_t *);

            [[nodiscard]] std::string dumpContents (amqp::CompositeFactory &) const;
            [[nodiscard]] std::string dumpSchema (schema::DumpTarget) const;
            [[nodiscard]] std::string dumpData () const;

            [[nodiscard]] pn_data_t * data() const;

            void readyPayload() const;

            void startComposite () const;
            void endComposite ();

            void startRestricted (const amqp::serializable::RestrictedSerializable &);
            void endRestricted (const amqp::serializable::RestrictedSerializable &);

            template<class T>
            T
            readPrimitive() const {
                DBG ("READ PRIM" << std::endl); // NOLINT
                return ReadPrimitive<T>::read(*this);
            }
    };

}

/******************************************************************************/

