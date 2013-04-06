#include "cdynamic/App.h"

void main()
{
	CDynamic::App& theApp = CDynamic::App::GetInstance();
	theApp.Start();

	return;
}