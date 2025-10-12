#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(args)
{
	if (args.empty())
		return;

	for (const auto& item : args)
	{
		m_Argument.append(item);
		m_Argument.push_back(' ');
	}
	//remove space at the end
	m_Argument.pop_back();
	
}

void CPrint::PrintUsageInfo()
{
	std::cout << R"(["PRINT"]
Usage: 
fsclt print version -> [prints Tool version]
fsclt print help -> [prints usefull help]
fsclt print info command all -> [prints the usage info of all commands]
fsclt prints info command [CommandName] -> [prints the Usage Info of [CommandName]])";

	std::cout << "\n\n";

	std::cout.flush();
}
void CPrint::Execute()
{
	if (m_Argument == "info version")
	{
		std::cout << "Version 0.1.0\n";
		std::cout.flush();
	}
	else if (m_Argument == "info command all")
	{
		const std::vector<BaseCommand*> buffer = FSCLT::Get().GetAllCommands();
		for (const auto& item : buffer)
		{
			item->PrintUsageInfo();
		}
	}
	else
	{
		std::cerr << "Undefined Parameters\n";
	}
}