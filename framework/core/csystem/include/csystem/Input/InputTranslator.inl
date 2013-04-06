namespace CSystem
{      
    template< class InputCommandIdentifier >  
    void InputTranslator< InputCommandIdentifier >::LoadFromXML( const CFoundation::String& _strFile )
    {
        InputTranslatorBase::LoadFromXML( _strFile, CFoundation::EnumConverter< InputCommandIdentifier >::GetInstance() );
    }

    template< class InputCommandIdentifier >  
    bool InputTranslator< InputCommandIdentifier >::WasExecuted( InputCommandIdentifier _eCommand, const InputState& _state ) const
    {
        return InputTranslatorBase::WasExecuted( _eCommand, _state );
    }
}
