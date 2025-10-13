#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(args)
{

}

void CPrint::PrintUsageInfo()
{
	std::cout << R"(["PRINT"]
Usage: 
fsclt print version -> [prints Tool version]
fsclt print help -> [prints usefull help]
fsclt print info command all -> [prints the usage info of all commands]
fsclt prints info command [CommandName] -> [prints the Usage Info of [CommandName]])";
}
void CPrint::Execute()
{



}