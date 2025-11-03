#include "PCH.hpp"
#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"
#include "FilesystemFormatHelper.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	//prints out the tool version
	BIND_COMMAND(std::vector<std::string>({ "info", "version" }), HandlePrintVersion);

	//prints out the usage of all commands or prints only 1 or more command usage infp 
	BIND_COMMAND(std::vector<std::string>({ "command", ARG_PARAM_FLAGS, ARG_MULTI_INP }), HandlePrintInfoCommands);
	//prints out the file or dir info
	BIND_COMMAND(std::vector<std::string>({ "info", "element", ARG_PARAM_FLAGS, ARG_MULTI_INP }), HandlePrintInfoElement);

	//searches a file and tells if it exists
	BIND_COMMAND(std::vector<std::string>({ "search", ARG_PARAM_FLAGS, ARG_MULTI_INP }), HandleSearch);

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

bool CPrint::HandlePrintVersion(const std::vector<std::string>& UserArgs, uint8_t ParamFlag)
{
	OutputLog::Get().SendMessage(TOOL_VERSION, 1);
	
	return true;
}

bool CPrint::HandlePrintInfoCommands(const std::vector<std::string>& UserArgs, uint8_t ParamFlag)
{
	OutputLog& log = OutputLog::Get();

	if (ParamFlag & EFLAG_PARAM::EFLAG_INFO && ParamFlag & EFLAG_PARAM::EFLAG_LIST)
	{
		log.ReportStatus("You cant combine both flags: [-i] and [-l].", MessageType::EERROR);
		return false;
	}

	switch (ParamFlag)
	{
	case EFLAG_INFO:
		return PrintCommandsWithName(UserArgs);
		break;

	case EFLAG_LIST:
		for (const auto& item : FSCLT::Get().GetAllCommands())
		{
			item->PrintUsageInfo();
		}
		break;

	default:
		log.ReportStatus("No valid Parameter flag found. Use -i or -l flag.", MessageType::EERROR);
		return false;
		break;
	}

	return true;
}
bool CPrint::PrintCommandsWithName(const std::vector<std::string>& UserArgs)
{
	if (UserArgs.empty())
	{
		OutputLog::Get().ReportStatus("No valid paremeter(s) for listing Commands with name", MessageType::EERROR);
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
			OutputLog::Get().ReportStatus("Couldn't find command: [" + item + "]. Make sure you spelled it correctly.", MessageType::EERROR);
			return false;
		}
	}
}

bool CPrint::HandlePrintCommandList(const std::vector<std::string>& UserArgs, uint8_t ParamFlag)
{
	for (const auto& item : FSCLT::Get().GetAllCommands())
	{
		item->PrintUsageInfo();
	}
	return true;
}

bool CPrint::HandlePrintListDirectory(const std::vector<std::string>& UserArgs, uint8_t ParamFlag)
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

bool CPrint::HandlePrintInfoElement(const std::vector<std::string>& UserArgs, uint8_t ParamFlag)
{
	OutputLog& log = OutputLog::Get();
	FilesystemFormatHelper& FormatHelper = FilesystemFormatHelper::Get();
	
	if (ParamFlag & EFLAG_PARAM::EFLAG_LOC && ParamFlag & EFLAG_PARAM::EFLAG_RECURSIVE)
	{
		log.ReportStatus("You cant combine both flags: [-loc] and [-r].", MessageType::EERROR);
		return false;
	}
	
	std::vector<fs::path> pathsVec;
	switch (ParamFlag)
	{

	case EFLAG_LOC:
		pathsVec = DoDirIterate(FSCLT::Get().GetExecutePath());
		
		break;

	case EFLAG_RECURSIVE:
		pathsVec = DoRecursiveDirIterate("C:\\Users\\joelf\\Documents");
		break;

	default:
		log.ReportStatus("No valid Parameter flag found. Use -loc or -r flag.", MessageType::EERROR);
		return false;
		break;
	}
	if (UserArgs.empty())
	{
		log.ReportStatus("No valid paremeter(s) for getting element info", MessageType::EERROR);
		return false;
	}
	
	try
	{
		for (const auto& arg : UserArgs)
		{
			bool exists = false;
			for (const auto& item : pathsVec)
			{
				if (item.stem().string() == arg)
				{
					log.SendMessage(FormatHelper.FormatDirectoryInfo(item));
					exists = true;
				
				}
				
			}
			if (!exists)
			{
				log.SendMessage("File or Directory: [" + arg + "] doesnt exists", 1, Color::BLUE);
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

bool CPrint::HandleSearch(const std::vector<std::string>& UserArgs, uint8_t ParamFlag)
{
	OutputLog& log = OutputLog::Get();
	FilesystemFormatHelper& FormatHelper = FilesystemFormatHelper::Get();
	const fs::path executePath = FSCLT::Get().GetExecutePath();

	if (UserArgs.empty())
	{
		log.ReportStatus("Invalid paremeter(s) for search command.", MessageType::EERROR);
		return false;
	}

	if (ParamFlag & EFLAG_PARAM::EFLAG_RECURSIVE && ParamFlag & EFLAG_PARAM::EFLAG_LOC)
	{
		log.ReportStatus("You cant combine both flags: [-r] and [-loc].", MessageType::EERROR);
		return false;
	}

	std::vector<fs::path> dirIter;

	switch (ParamFlag)
	{
	case EFLAG_RECURSIVE:
		dirIter = std::move(DoRecursiveDirIterate(executePath));
		break;
		
	case EFLAG_LOC:
		dirIter = std::move(DoDirIterate(executePath));
		break;

	default:
		log.ReportStatus("No valid Parameter flag found. Use -r or -loc flag.", MessageType::EERROR);
		break;
	}

	//Jump over the flags
	for (size_t i = 0; i < UserArgs.size(); i++)
	{
		for (const auto& item : dirIter)
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
bool CPrint::CheckSameFlags(uint8_t base, EFLAG_PARAM params...)
{
	return base & params;
}
