#include "PCH.hpp"
#include "FSCLT.hpp"

int main(int argc, char* argv[])
{

	std::setlocale(LC_ALL, ".UTF8");

	//Convert char* to std::string
	std::vector<std::string> args;
	for (size_t i = 0; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	FSCLT fsclt(argc, args);
	bool result = fsclt.Run();
	
	if(!result)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}