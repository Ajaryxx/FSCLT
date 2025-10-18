#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	BIND_COMMAND(std::vector<std::string>({ "info", "version" }), HandlePrintVersion);
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTIINP }), HandlePrintCommands);
	BIND_COMMAND(std::vector<std::string>({ "list", "dir", ARG_MULTIINP}), HandlePrintDirectorys);
	BIND_COMMAND(std::vector<std::string>({ "list", "file", ARG_MULTIINP }), HandlePrintFiles);
}

void CPrint::PrintUsageInfo()
{
	FSCLT::Get().ReportMessage(R"(["PRINT"]
Usage:
fsclt print help ->[prints Tool version]
fsclt print help ->[prints usefull help]
fsclt print info command all ->[prints the usage info of all commands]
fsclt prints info command[CommandName] ->[prints the Usage Info of[CommandName]]);)");

}

bool CPrint::HandlePrintVersion(const std::vector<std::string>& UserArgs)
{
	FSCLT::Get().ReportMessage("version 0.1.0");
	
	return true;
}
bool CPrint::HandlePrintCommands(const std::vector<std::string>& UserArgs)
{
	if (UserArgs.empty())
	{
		for (const auto& item : FSCLT::Get().GetAllCommands())
		{
			item->PrintUsageInfo();
		}
	}
	else
	{
		for (const auto& item : UserArgs)
		{
			BaseCommand* cmd = FSCLT::Get().GetCommand(item);
			if (cmd)
			{
				cmd->PrintUsageInfo();
			}
			else
			{
				FSCLT::Get().ReportMessage("Couldn't find Command: " + item, MessageType::ERROR);
				return false;
			}
		}
	}
	return true;
}
bool CPrint::HandlePrintDirectorys(const std::vector<std::string>& UserArgs)
{
	return true;
}
bool CPrint::HandlePrintFiles(const std::vector<std::string>& UserArgs)
{
	return true;
}