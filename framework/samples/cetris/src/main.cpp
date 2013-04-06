#include "cetris/CetrisApp.h"

void main()
{
	Cetris::CetrisApp& theApp = Cetris::CetrisApp::GetInstance();
	theApp.Start();

	return;
}