template < class T >
SharedPtr< T > CCtrl::WindowManager::Create( CGui::WindowPtr _spWindow, CCtrl::WindowControllerPtr _spParent )
{
	// Create controller
	SharedPtr< T > spController( new T() );

	// Initialize controller
	spController->InitController( _spWindow, _spParent );

	// Add window
	m_lspWindows.push_back( spController );

	return spController;
}
