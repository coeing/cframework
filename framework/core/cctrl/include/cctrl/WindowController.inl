namespace CCtrl
{
    template < class T >
    SharedPtr< T > WindowController::Create( WindowControllerPtr _spParent )
    {
	    // Create window
        CGui::WindowPtr pWindow = Create( T::GetXMLFileName() );
	    CF_WARN_IF( pWindow == NULL, L"WindowController::CreateScreen: Could not create window" );
	    if ( pWindow == NULL )
	    {
	        return SharedPtr< T >();
	    }

        // Create controller
	    SharedPtr< T > spController = WindowManager::GetInstance().Create< T >( pWindow, _spParent );

        // Init window
        spController->InitWindow();

	    return spController;
    }
}
