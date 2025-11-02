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

	//info specific
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTIINP }), HandlePrintInfoCommands);
	BIND_COMMAND(std::vector<std::string>({ "info", "element", ARG_MULTIINP }), HandlePrintInfoElement);

	//list specific
	BIND_COMMAND(std::vector<std::string>({ "list", "dir" }), HandlePrintListDirectory);
	BIND_COMMAND(std::vector<std::string>({ "list", "command", }), HandlePrintCommandList);

	//search specific
	BIND_COMMAND(std::vector<std::string>({ "search", ARG_MULTIINP }), HandleSearch);

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

bool CPrint::HandlePrintInfoElement(const std::vector<std::string>& UserArgs)
{
	OutputLog& log = OutputLog::Get();
	FilesystemFormatHelper& FormatHelper = FilesystemFormatHelper::Get();

	if (UserArgs.empty())
	{
		log.ReportStatus("No valid paremeter(s) found", MessageType::EERROR);
		return false;
	}

	try
	{
		for (const auto& item : UserArgs)
		{
			const fs::path p = FSCLT::Get().GetExecutePath() / item;
			if (fs::exists(p))
			{
				log.SendMessage(FormatHelper.FormatDirectoryInfo(p));
			}
			else
			{
				log.ReportStatus("Couldn't find directory or file: [" + item + "]", MessageType::EERROR);
				return false;
			}
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
	OutputLog& log = OutputLog::Get();
	FilesystemFormatHelper& FormatHelper = FilesystemFormatHelper::Get();

	const fs::path executePath = FSCLT::Get().GetExecutePath();

	if (UserArgs.empty())
	{
		log.ReportStatus("No valid paremeter(s) found", MessageType::EERROR);
		return false;
	}
	std::vector<std::string> extractedFlags = ExtractParamFlags(UserArgs);
	uint8_t flags = GetParamFlagsAsFlag(extractedFlags);
	std::vector<fs::path> recrIt;

	switch (flags)
	{
	case EFLAG_RECURSIVE:
		recrIt = std::move(DoRecursiveDirIterate(executePath));
		break;
		
	default:
		recrIt = std::move(DoDirIterate(executePath));
		break;
	}

	//Jump over the flags
	for (size_t i = extractedFlags.size(); i < UserArgs.size(); i++)
	{
		for (const auto& item : recrIt)
		{
			if (item.stem().string() == UserArgs[i])
			{
				log.SendMessage(FormatHelper.FormatDirectoryInfo(item));
			}
		}
	}
	
	return true;
}

std::vector<std::filesystem::path> CPrint::DoRecursiveDirIterate(const std::filesystem::path& searchPath) const
{
	std::vector<fs::path> paths;
	for (const auto& item : fs::recursive_directory_iterator(searchPath))
	{
		paths.push_back(item);
	}
	return paths;
}
std::vector<std::filesystem::path> CPrint::DoDirIterate(const std::filesystem::path& searchPath) const
{
	std::vector<fs::path> paths;
	for (const auto& item : fs::directory_iterator(searchPath))
	{
		paths.push_back(item);
	}
	return paths;
}