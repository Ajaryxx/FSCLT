#include <iostream>
#include "Commands/CVersion.hpp"

void CVersion::PrintUsageInfo()
{
}

void CVersion::Execute()
{
	std::cout << "Version 1.0\n";
}
