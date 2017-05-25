#include "SessionInspector.h"
#include <iostream>

void main()
{
	SessionInstector si;

	std::cout << si.IsCurrentSessionRemoteable();
	std::cout << si.DetectingSessionEvents();
}