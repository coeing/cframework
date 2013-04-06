#include "ced/CedApp.h"

int main()
{
	Ced::CedApp& theApp = Ced::CedApp::GetInstance();
	theApp.Start();

	return 0;
}
