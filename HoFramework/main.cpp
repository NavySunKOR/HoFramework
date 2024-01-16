#include <iostream>
#include "Application.h"

using namespace std;


int main()
{
	Application App;

	if (App.IsInitialized() == false)
	{
		return -1;
	}

	return App.Run();
}