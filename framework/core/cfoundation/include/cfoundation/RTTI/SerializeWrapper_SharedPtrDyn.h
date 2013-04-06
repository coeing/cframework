#ifndef CFOUNDATION_SERIALIZE_WRAPPER_SHAREDPTR_DYN
#define CFOUNDATION_SERIALIZE_WRAPPER_SHAREDPTR_DYN

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Debug/Assert.h"
#include "cfoundation/RTTI/rtti.h"

namespace CFoundation
{
    class Serializer;

    class SerializeWrapper_SharedPtrDyn
    {
    public:
        /// Serialize object
        template < class Object >
        static void     Serialize( Serializer& _Serializer, SharedPtr< Object >& _spObject )
        {
            bool bPresent = _spObject != NULL;
            _Serializer.Serialize( bPresent );
            if ( bPresent )
            {
                if ( _Serializer.IsSaving() )
                {
                    // Save object type
                    RTTItypeid type_id = TYPE_INFO( _spObject.get() );
                    String strType( type_id.getname() );
                    _Serializer.Serialize( strType );
                }
                else
                {
                    // Get object type
                    String strType;
                    _Serializer.Serialize( strType );
                    RTTIdyntypeid type_id( strType.c_str() );

                    // Create object
                    _spObject = SharedPtr< Object >( (Object*)STATIC_TYPE_INFO( Object ).create( type_id ) );
                    CF_WARN_IF( _spObject == NULL, "SerializeWrapper_SharedPtrDyn: Couldn't create an RTTI object while loading." );
                }

                SAFE_CALL( _spObject )->Serialize( _Serializer );
            }
        }
    };
}

#endif
