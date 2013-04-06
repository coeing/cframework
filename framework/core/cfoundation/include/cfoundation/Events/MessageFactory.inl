namespace CFoundation
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PluggableMessageFactory
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////
    template< class KeyType >
    typename PluggableMessageFactory< KeyType >::MessageMap PluggableMessageFactory< KeyType >::m_mapRegistry;

    template< class KeyType >
    PluggableMessageFactory< KeyType >::Maker::Maker( const KeyType& _Key ) 
    { 
        m_mapRegistry.insert( std::make_pair( _Key, this ) ); 
    }

    template< class KeyType >
    BaseMessagePtr PluggableMessageFactory< KeyType >::Create( const KeyType& _Key )
    {
        MessageMap::iterator it = m_mapRegistry.find( _Key );
        if ( it != m_mapRegistry.end() )
        {
            return (*it).second->OnCreate();
        }
        else
        {
            return BaseMessagePtr();
        }
    }
};
