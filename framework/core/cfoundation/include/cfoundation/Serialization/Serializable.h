#ifndef CFOUNDATION_SERIALIZABLE_H
#define CFOUNDATION_SERIALIZABLE_H

namespace CFoundation
{
    class Serializer;

	class Serializable
	{
	public:
        /// Serializes the object data with the passed serializer
        virtual void    Serialize( Serializer& /*_clSerializer*/ ) {};

        /// Saves the object data with the passed serializer
		virtual void    Save( Serializer& /*_clSerializer*/ ) const {};

        /// Loads the object data from the passed serializer
		virtual void    Load( const Serializer& /*_clSerializer*/ ) {};
	};
}

#endif
