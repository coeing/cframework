//#include <vld.h>

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/XML/XMLParser.h"

//! Base class of string callbacks
typedef Delegate1< const CFoundation::String& > StringDelegate;
typedef SharedPtr< StringDelegate > StringDelegatePtr;

class StringCallbackBase
{
public:
    //! Callback
    virtual void    DoCallback( const CFoundation::String& _strParam ) = 0;
};

typedef SharedPtr< StringCallbackBase > StringCallbackPtr;

enum TestStringEvent
{
    TEST_STRING_EVENT_NONE,

    TEST_STRING_EVENT_A,
    TEST_STRING_EVENT_B,

    TEST_STRING_EVENT_END
};

class DelegateInvoker : public CFoundation::Delegate1Invoker< TestStringEvent, const CFoundation::String& >
{
public:
    DELEGATE1_INVOKER( TestStringEvent, const CFoundation::String& );
};

class CallbackHandler
{
public:
    /// Callback for event A
    void        CallbackA( const CFoundation::String& _strParam )
    {
        std::cout << "Callback A: " << _strParam << std::endl;
    }

    /// Callback for event B
    void        CallbackB( const CFoundation::String& _strParam )
    {
        std::cout << "Callback B: " << _strParam << std::endl;
    }
};

class PA
{
};

class A
{
public:
    virtual void Dispatch( const PA& _param );
};

class PB
{
};

class B
{
public:
    virtual void Dispatch( const PB& _param );
};

#define DELEGATE_INVOKER_IMPL0( _Class, _Enum ) void Dispatch( _Enum _event ) { _Class::Dispatch( _event ); }

class C : public virtual A, public virtual B
{
public:
    DELEGATE_INVOKER_IMPL0( A, PA );
    DELEGATE_INVOKER_IMPL0( B, PB );
};

int main( int argc, char** argv )
{
    //C c;
    //PA pa;
    //c.Dispatch( pa );

    // Class Color
    CFoundation::Color clColor1;
    CFoundation::Color clColor2( 1.0f, 0, 0 );

    std::cout << clColor1.GetHexString() << std::endl;
    std::cout << clColor2.GetHexString() << std::endl;

    // Class String
    CFoundation::String sHello = "Hello";
    CFoundation::String sWorld = "World";
    std::cout << sHello;
    std::cout << " " + sWorld + L"!" << std::endl;

    // Class XMLParser
    CFoundation::XMLParser& clParser = CFoundation::XMLParser::GetInstance();
    CFoundation::XMLDocumentPtr spDocument = clParser.ParseFile( "data/test.xml" );
    if ( spDocument == NULL )
    {
        std::cout << "XML file Test.xml could not be parsed." << std::endl;
    }
    else
    {
        CFoundation::XMLNodePtr pNode = spDocument->GetRootElement();
        while ( pNode != NULL )
        {
            if ( pNode->IsElement() )
            {
                CFoundation::XMLElementPtr pElement = static_pointer_cast< CFoundation::XMLElement >( pNode );
                if ( pElement != NULL )
                {
                    std::cout << pElement->GetName() << std::endl;
                }
            }

            CFoundation::XMLNodePtr pSibling = pNode->GetNextSibling();
            if ( pSibling != NULL )
            {
                pNode = pSibling;
            }
            else
            {
                pNode = pNode->GetFirstChild();
            }
        }
    }

    // Class DelegateInvoker
    DelegateInvoker clInvoker;
    CallbackHandler clHandler;

    // Create callbacks
    StringDelegatePtr spDelegateA( new StringDelegate( &clHandler, &CallbackHandler::CallbackA ) );
    StringDelegatePtr spDelegateB( new StringDelegate( &clHandler, &CallbackHandler::CallbackB ) );

    // Register callbacks
    clInvoker.RegisterDelegate( TEST_STRING_EVENT_A, spDelegateA );
    clInvoker.RegisterDelegate( TEST_STRING_EVENT_B, spDelegateB );

    // Invoke callbacks
    clInvoker.DispatchEvent( TEST_STRING_EVENT_B, "Get this" );
    clInvoker.DispatchEvent( TEST_STRING_EVENT_A, "Get that" );

    getchar();

    return 0;
}
