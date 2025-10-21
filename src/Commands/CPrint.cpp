#include "PCH.hpp"
#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"

namespace fs = std::filesystem;

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	BIND_COMMAND(std::vector<std::string>({ "info", "version" }), HandlePrintVersion);
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTIINP }), HandlePrintCommands);
	BIND_COMMAND(std::vector<std::string>({ "list", "dir", ARG_MULTIINP}), HandlePrintDirectorys);
	BIND_COMMAND(std::vector<std::string>({ "list", "file", ARG_MULTIINP }), HandlePrintFiles);
}

void CPrint::PrintUsageInfo()
{
	OutputLog::Get().ReportStatus(R"(["PRINT"]
Usage:
fsclt print help ->[prints Tool version]
fsclt print help ->[prints usefull help]
fsclt print info command all ->[prints the usage info of all commands]
fsclt prints info command[CommandName] ->[prints the Usage Info of[CommandName]]);)");

}

bool CPrint::HandlePrintVersion(const std::vector<std::string>& UserArgs)
{
	//FSCLT::Get().ReportMessage("version 0.1.0");
	
	return true;
}
bool CPrint::HandlePrintCommands(const std::vector<std::string>& UserArgs)
{
	bool FoundDirecorys = false;
	if (UserArgs.empty())
	{
		for (const auto& item : FSCLT::Get().GetAllCommands())
		{
			item->PrintUsageInfo();
			FoundDirecorys = true;
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
				FoundDirecorys = true;
			}
			else
			{
				OutputLog::Get().ReportStatus("Couldn't find Command: " + item, MessageType::ERROR);
				return false;
			}
		}
	}
	return true;
}
bool CPrint::HandlePrintDirectorys(const std::vector<std::string>& UserArgs)
{
	bool FoundDirecorys = false;

	OutputLog::Get().ReportStatus("There are following directorys: ");

	if (UserArgs.empty())
	{
		for (const auto& item : fs::directory_iterator(FSCLT::Get().GetExecutePath()))
		{
			if (item.is_directory());
			OutputLog::Get().SendMessage(item.path().filename().u8string());
		}
	}
	else
	{
		for (const auto& arg : UserArgs)
		{
			for (const auto& dirEntry : fs::directory_iterator(FSCLT::Get().GetExecutePath()))
			{
				if (dirEntry.is_regular_file())
				{
					if (dirEntry.path().extension() == arg)
					{
						OutputLog::Get().SendMessage(dirEntry.path().filename().u8string());
					}
				}
			}
		}
	}
	if (!FoundDirecorys)
		OutputLog::Get().SendMessage("No direcorys found", 0, Color::CYAN);

	return true;
}
bool CPrint::HandlePrintFiles(const std::vector<std::string>& UserArgs)
{
	return true;
}