#include "PCH.hpp"
#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"

namespace fs = std::filesystem;

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	BIND_COMMAND(std::vector<std::string>({ "info", "version" }), HandlePrintVersion);
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTIINP }), HandlePrintCommands);

	BIND_COMMAND(std::vector<std::string>({ "info", "dir", ARG_MULTIINP }), HandlePrintInfoDirectory);
	BIND_COMMAND(std::vector<std::string>({ "info", "file", ARG_MULTIINP }), HandlePrintInfoFile);

	BIND_COMMAND(std::vector<std::string>({ "list", "dir"}), HandlePrintListDirectorys);
	BIND_COMMAND(std::vector<std::string>({ "list", "file", ARG_MULTIINP }), HandlePrintListFiles);
}

void CPrint::PrintUsageInfo()
{
	OutputLog::Get().SendMessage(R"(["PRINT"]
Usage:
fsclt print info version -> [prints Tool version]
fsclt print info command [COMMAND_NAMES] -> [prints usage help for command]
fsclt print list dir [EXTENSION] -> [lists all diretorys with a speficic extension]
fsclt prints list file [EXTENSION] ->[prints all files with a spefific extension])", 
1, 
Color::CYAN);
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
bool CPrint::HandlePrintListDirectorys(const std::vector<std::string>& UserArgs)
{
	std::vector<fs::path> buffer;
	const std::string ExecutePath = FSCLT::Get().GetExecutePath();
	OutputLog::Get().ReportStatus("There are following directorys: ");

	bool succses = true;
	try
	{
		for (const auto& item : fs::directory_iterator(ExecutePath))
		{
			if (item.is_directory())
				buffer.push_back(item);
		}
	}
	catch (const fs::filesystem_error& err)
	{
		OutputLog::Get().ReportStatus("ERROR: " + std::string(err.what()), MessageType::ERROR);
		succses = false;
	}

	if (buffer.empty())
		OutputLog::Get().SendMessage("No directories found", 0, Color::CYAN);
	else
		OutputLog::Get().PrintDirInfo(buffer);

	return succses;
}
bool CPrint::HandlePrintListFiles(const std::vector<std::string>& UserArgs)
{
	return true;
}
bool CPrint::HandlePrintInfoDirectory(const std::vector<std::string>& UserArgs)
{
	return true;
}
bool CPrint::HandlePrintInfoFile(const std::vector<std::string>& UserArgs)
{
	return true;
}