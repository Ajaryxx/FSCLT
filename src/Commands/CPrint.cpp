#include "PCH.hpp"
#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"
#include "FilesystemFormatHelper.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	//tool specific
	BIND_COMMAND(std::vector<std::string>({ "info", "version" }), HandlePrintVersion);

	//command specific
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTIINP }), HandlePrintInfoCommands);
	BIND_COMMAND(std::vector<std::string>({ "list", "command",}), HandlePrintCommandList);

	//dir list specific
	BIND_COMMAND(std::vector<std::string>({ "list", "dir" }), HandlePrintListDirectory);

	//dir info specific
	BIND_COMMAND(std::vector<std::string>({ "info", "dir", ARG_MULTIINP }), HandlePrintInfoDirectory);
	BIND_COMMAND(std::vector<std::string>({ "info", "file", ARG_MULTIINP }), HandlePrintInfoFile);

	//Search specific
	BIND_COMMAND(std::vector<std::string>({ "print", "search", ARG_MULTIINP }), HandleSearch);

;
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
	OutputLog::Get().SendMessage(TOOL_VERSION, 1);
	
	return true;
}

bool CPrint::HandlePrintInfoCommands(const std::vector<std::string>& UserArgs)
{
	OutputLog& log = OutputLog::Get();
	if (UserArgs.empty())
	{
		log.ReportStatus("No valid paremeter(s) found", MessageType::EERROR);
		return false;
	}

	for (const auto& item : UserArgs)
	{
		if (BaseCommand* cmd = FSCLT::Get().GetCommand(item))
		{
			cmd->PrintUsageInfo();
		}
		else
		{
			log.ReportStatus("Couldn't find command: [" + item + "]. Make sure you spelled it correctly.", MessageType::EERROR);
			return false;
		}
	}
	return true;
}
bool CPrint::HandlePrintCommandList(const std::vector<std::string>& UserArgs)
{
	for (const auto& item : FSCLT::Get().GetAllCommands())
	{
		item->PrintUsageInfo();
	}
	return true;
}
bool CPrint::HandlePrintListDirectory(const std::vector<std::string>& UserArgs)
{
	OutputLog& log = OutputLog::Get();
	FilesystemFormatHelper& FormatHelper = FilesystemFormatHelper::Get();
	try
	{
		for (const auto& item : fs::directory_iterator(FSCLT::Get().GetExecutePath()))
		{
			log.SendMessage(FormatHelper.FormatDirectoryInfo(item));
		}
	}
	catch (const fs::filesystem_error& err)
	{
		log.ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR);
		return false;
	}

	return true;
}
bool CPrint::HandleSearch(const std::vector<std::string>& UserArgs)
{
	return true;
}
std::vector<fs::path> CPrint::CheckFileExtInDir(const std::filesystem::path& dir, const std::string& ext) const
{
	std::vector<fs::path> buffer;
	for (const auto& item : fs::directory_iterator(dir))
	{
		if (item.path().extension().string() == ext)
		{
			buffer.push_back(item);	
		}
	}
	return buffer;
}

bool CPrint::HandlePrintInfoDirectory(const std::vector<std::string>& UserArgs)
{
	return true;
}
bool CPrint::HandlePrintInfoFile(const std::vector<std::string>& UserArgs)
{
	return true;
}