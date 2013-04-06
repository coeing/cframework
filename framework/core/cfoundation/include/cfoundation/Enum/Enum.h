#ifndef CFOUNDATION_ENUM_H
#define CFOUNDATION_ENUM_H

#include "cfoundation/Enum/EnumBase.h"

namespace CFoundation
{
    class Enum : public EnumBase
    {
    public:
        /// Constructor
        Enum()
            :m_i32Value( 0 )
        {
        }

        Enum( Integer32 _i32Value )
            : m_i32Value( _i32Value )
        {
        }

        bool operator<( const Enum& _rhs )
        {
            return m_i32Value < _rhs.m_i32Value;
        }

        bool operator>( const Enum& _rhs )
        {
            return m_i32Value > _rhs.m_i32Value;
        }

        friend bool operator<( const Enum& _lhs, const Enum& _rhs );

    private:
        /// Value
        Integer32   m_i32Value;
    };

    bool operator<( const Enum& _lhs, const Enum& _rhs )
    {
        return _lhs.m_i32Value < _rhs.m_i32Value;
    }
}

#define ENUM1( _Name, _Value1 ) class _Name : public CFoundation::Enum \
                                { \
                                    public: \
                                        enum Value{ VALUE_NONE, _Value1, VALUE_END }; \
                                        _Name() {} \
                                        _Name( Value _eValue ) : Enum( _eValue ) {} \
                                };

#endif
