#include "PCH.hpp"
#include "FSCLT.hpp"

int main(int argc, char* argv[])
{
	FSCLT fsclt(argc, argv);
	bool result = fsclt.Run();

	if(!result)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}