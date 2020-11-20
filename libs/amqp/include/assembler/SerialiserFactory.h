#pragma once

/******************************************************************************/

#include "amqp/src/ModifiableAMQPBlobImpl.h"

/******************************************************************************/

namespace amqp::serializable {

    class Serializable;
    class RestrictedSerializable;

    template<typename, typename> class SerializableVector;
    template<typename, typename, typename, typename> class SerializableMap;

}

/******************************************************************************/

namespace amqp::assembler {
    using Serializable = amqp::serializable::Serializable;
    using RestrictedSerializable = amqp::serializable::RestrictedSerializable;

    class SerialiserFactory {
        private :
            /*
             * Since we can't partially specialise functions we need
             * to pull out the writing of single values into two functions. Use
             * basic meta-programing to switch between primitives and
             * composites
             */
            template<typename T,  bool = std::is_base_of<Serializable , T>::value>
            struct SingleWriter {
                static void write (T propertyValue_, ModifiableAMQPBlob & blob_, const SerialiserFactory & sf_) {
                    DBG ("  SingleWriter::" << javaTypeName<T>() << " - primitive" << std::endl);
                    dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_).writePrimitiveSingle<T>(
                        propertyValue_);
                }
            };

            /*
             * Specialisation to handle the composite case, basically anything which
             * implements Serializable.
             */
            template<typename T>
            struct SingleWriter<T, true> {
                static void write (T propertyValue_, ModifiableAMQPBlob & blob_, const SerialiserFactory & sf_) {
                    DBG ("  SingleWriter::" << javaTypeName<T>() << " - serialise" << std::endl);
                    propertyValue_.serialise (sf_, blob_);
                }
            };

            /*
             * Similar to the above, save for properties of composites
             */
            template<typename T,  bool = std::is_base_of_v<Serializable , std::remove_pointer_t<T>>, bool = std::is_pointer_v<T>>
            struct PropertyWriter {
                static void write (T propertyValue_, const std::string & propertyName_,
                    const Serializable & clazz_, ModifiableAMQPBlob & blob_, const SerialiserFactory & sf_
                ) {
                    dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_).writePrimitive<T> (
                        propertyValue_, propertyName_, clazz_);
                }
            };

            template<typename T,  bool = std::is_base_of_v<Serializable , std::remove_pointer_t<T>>>
            struct PropertyReader {
                static T read (const AMQPBlob & blob_, const SerialiserFactory & sf_) {
                    return blob_.readPrimitive<T> ();
                }
            };

            template<typename T>
            struct PropertyReader<T, true> {
                static T read (const AMQPBlob & blob_, const SerialiserFactory & sf_) {
                    return blob_.readComposite<T> ();
                }
            };

            template<typename T>
            struct NonPrimWriter {
                static void write (
                    const std::string & type_, const std::string & propertyName_,
                    const Serializable & parent_, internal::ModifiableAMQPBlobImpl & blob_
                ) {
                    blob_.writeComposite (propertyName_, type_, parent_);
                }
            };

            template<typename A, typename B>
            struct NonPrimWriter<amqp::serializable::SerializableVector<A, B>> {
                static void write (
                    const std::string & type_, const std::string & propertyName_,
                    const Serializable & parent_, internal::ModifiableAMQPBlobImpl & blob_
                ) {
                    blob_.writeRestricted (propertyName_, type_, parent_);
                }
            };

            template<typename A, typename B, typename C, typename D>
            struct NonPrimWriter<amqp::serializable::SerializableMap<A, B, C, D>> {
                static void write (
                    const std::string & type_, const std::string & propertyName_,
                    const Serializable & parent_, internal::ModifiableAMQPBlobImpl & blob_
                ) {
                    blob_.writeRestricted (propertyName_, type_, parent_);
                }
            };

            /*
             * specialisation for non pointer composites
             */
            template<typename T>
            struct PropertyWriter<T, true, false> {
                static void write (
                    T propertyValue_, const std::string & propertyName_,
                    const Serializable & parent_, ModifiableAMQPBlob & blob_, const SerialiserFactory & sf_
                ) {
                    auto type = javaTypeName<std::remove_reference_t<std::remove_pointer_t<T>>>();
                    auto & blob = dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_);

                    NonPrimWriter<T>::write (type, propertyName_, parent_, blob);

                    propertyValue_.serialise (sf_, blob_);
                }
            };

            /*
             * specialisation for pointer composites
             */
            template<typename T>
            struct PropertyWriter<T, true, true> {
                static void write (
                    T propertyValue_, const std::string & propertyName_,
                    const Serializable & parent_, ModifiableAMQPBlob & blob_, const SerialiserFactory & sf_
                ) {
                    auto type = javaTypeName<std::remove_reference_t<std::remove_pointer_t<T>>>();
                    auto & blob = dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_);

                    NonPrimWriter<T>::write (type, propertyName_, parent_, blob);

                    if (propertyValue_) {
                        propertyValue_->serialise (sf_, blob_);
                    } else {
                        blob.writeNull (propertyName_, type, parent_);
                    }
                }
            };

        public :
            [[nodiscard]] virtual uPtr<ModifiableAMQPBlob> blob() const = 0;

            static void startComposite (const Serializable & clazz_, ModifiableAMQPBlob & blob_) {
                dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_).startComposite (clazz_);
            }

            static void endComposite (const Serializable & clazz_, ModifiableAMQPBlob & blob_) {
                dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_).endComposite (clazz_);
            }

            static void startRestricted (
                const RestrictedSerializable & clazz_,
                ModifiableAMQPBlob & blob_
            )  {
                dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_).startRestricted (clazz_);
            }

            static void endRestricted (
                const RestrictedSerializable & clazz_,
                ModifiableAMQPBlob & blob_
            )  {
                dynamic_cast<internal::ModifiableAMQPBlobImpl &>(blob_).endRestricted (clazz_);
            }

            template<typename T>
            void write (
                T propertyValue_,
                const std::string & propertyName_,
                const Serializable & clazz_,
                ModifiableAMQPBlob & blob_
            ) const {
                PropertyWriter<T>::write (propertyValue_, propertyName_, clazz_, blob_, *this);
            }

            template<typename T>
            void writeSingle (T propertyValue_, const Serializable & clazz_,
                ModifiableAMQPBlob & blob_
            ) const {
                SingleWriter<T>::write (propertyValue_, blob_, *this);
            }

            template<typename T>
            T
            read (
                const AMQPBlob & blob_
            ) const {
                DBG (__FUNCTION__ << std::endl); // NOLINT
                return PropertyReader<T>::read (blob_, *this);
            }

            template<typename T>
            T
            deserialise (const AMQPBlob & blob_) {
                DBG (__FUNCTION__ << std::endl); // NOLINT
                blob_.readyPayload ();
                T::deserialise (blob_);
                return T { *this, blob_ };
            }
    };

}

/******************************************************************************/
