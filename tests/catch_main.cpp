#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_RUNNER

#include "catch.hpp"

#include <iostream>

int main(int argc, char *argv[]) 
{
	Catch::Session session;  // Primary Catch object
	int catchresult = session.applyCommandLine(argc, argv);

	if (!catchresult)  // Continue only if no command-line error
	{
		// Run test suites
		std::cout << "Tests for C++ to PostScript converter program" << std::endl;
		std::cout << "CS 372 Software Construction" << std::endl;
		catchresult = session.run();
		std::cout << "End of tests program" << std::endl;
	}

	// Wait for user
	std::cout << "Press ENTER to quit ";
	while (std::cin.get() != '\n');

	// Program return value is return code from Catch
	return catchresult;
}