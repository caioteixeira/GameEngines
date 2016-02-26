// Main.cpp : Defines the entry point for the console application.
//

#include "Lab1Tests.h"

int main(int argc, char* argv[])
{
	// Just run the unit tests
	TestFixtureFactory::theInstance().runTests();

#if _WIN32
	std::cout << "Press enter to continue..." << std::endl;
	getchar();
#endif
	return 0;
}

