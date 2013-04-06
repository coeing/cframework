#ifndef CFOUNDATION_SERIALIZATION_CONTEXT_H
#define CFOUNDATION_SERIALIZATION_CONTEXT_H

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/RTTI/SerializeWrapper_SharedPtrDyn.h"
#include "cfoundation/Serialization/Serializer.h"

namespace CFoundation
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SerializationContext
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

	class SerializationContext
	{
	public:
        /// Serialize object
        template < class Object >
        void    Serialize( Serializer& _Serializer, SharedPtr< Object >& _spObject );

        /// Register serialize method for pointer
        template < class Object >
        void    RegisterSerializeMethod( SharedPtr< Delegate2< Serializer&, SharedPtr< Object >& > > _spDelegate );

    private:
        typedef SharedPtr< void > VoidPtr;
        typedef std::map< RTTItypeid, VoidPtr > SerializeMethodMap;

        SerializeMethodMap  m_mapSerializeMethods;
	};

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SerializeWrapper_Context
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SerializeWrapper_Context
    {
    public:
        /// Serialize object
        template < class Object >
        static void     Serialize( Serializer& _Serializer, SharedPtr< Object >& _spObject );
    };
}

#include "cfoundation/Serialization/SerializationContext.inl"

#endif
