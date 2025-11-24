#include "PCH.hpp"
#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"
#include "FilesystemUtilityHelper.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	//prints out the current Tool version
	BIND_COMMAND(std::vector<std::string>({ "info", "version"}), HandlePrintOutVersion);

	//prints out the usage from command(s)
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTI_INP }), HandlePrintOutCommandInfo);

	//prints out the directory list
	BIND_COMMAND(std::vector<std::string>({ "list", "dir", ARG_MULTI_INP }), HandlePrintListDirectory);

	//prints out the info if this/these element(s)
	BIND_COMMAND(std::vector<std::string>({ "info", "element", ARG_MULTI_INP }), HandlePrintInfoElement);
}

void CPrint::PrintUsageInfo() const
{
	OutputLog::Get().SendOutput("TEST_PRINT");
}

bool CPrint::HandlePrintOutVersion(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	OutputLog::Get().SendOutput("Tool verion: " + TOOL_VERSION);
	return true;
}

bool CPrint::HandlePrintOutCommandInfo(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	if (!CheckParemetersFound(UserArgs, "HANDLE_PRINT_CMD_INFO"))
		return false;

	OutputLog& log = OutputLog::Get();
	const bool PrintAll = UserArgs[0] == "all";

	if (PrintAll)
	{
		//print all
		const std::vector<BaseCommand*>& cmds = FSCLT::Get().GetAllCommands();

		for (const auto& item : cmds)
			item->PrintUsageInfo();
	}
	else
	{
		//print commands with name
		for (const auto& item : UserArgs)
		{
			if (const BaseCommand* command = FSCLT::Get().GetCommand(item))
				command->PrintUsageInfo();

			else
			{
				log.ReportStatus("Command: " + item + " wasnt found", MessageType::EERROR);
				return false;
			}
		}
	}

	return true;
}

bool CPrint::HandlePrintListDirectory(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	OutputLog& log = OutputLog::Get();
	FilesystemUtilityHelper& utilityHelper = FilesystemUtilityHelper::Get();

	std::vector<fs::path> path;
	if (UserArgs.empty())
	{
		path = GetDirectoryLocalPaths(FSCLT::Get().GetExecutePath());
		std::vector<std::string> stringPaths(path.size());
		
		for (const auto& item : path)
			std::transform(path.begin(), path.end(), stringPaths.begin(), [&](const fs::path& str) 
				{ 
					
					std::string strPath = str.string();
					std::string type;
					if (fs::is_directory(strPath))
						type = "[Folder] ";
					else
						type = "[File] ";
					
					strPath.insert(0, type);

					return strPath;
				});

		log.PrintList(stringPaths, "Directory List", Color::MAGENTA);
	}
	return true;
}

bool CPrint::HandlePrintInfoElement(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	if (!CheckParemetersFound(UserArgs, "HANDLE_PRINT_INFO_ELEMENT"))
		return false;

	OutputLog& log = OutputLog::Get();

	for (const auto& arg : UserArgs)
	{
		bool Found = false;
		for (const auto& element : GetDirectoryLocalPaths(FSCLT::Get().GetExecutePath()))
		{
			if (element.stem() == arg)
			{
				log.SendOutput(FilesystemUtilityHelper::Get().FormatDirectoryInfo(element));
				Found = true;
			}
		}
		if (!Found)
		{
			log.ReportStatus("Element name: [" + arg + "] wasn't found.", MessageType::EERROR);
			return false;
		}
	}

	return true;

}

std::vector<std::filesystem::path> CPrint::GetDirectoryRecursivePaths(const std::filesystem::path& searchPath) const
{
	std::vector<fs::path> paths;
	for (const auto& item : fs::recursive_directory_iterator(searchPath))
	{
		paths.push_back(item);
	}
	return paths;
}

std::vector<std::filesystem::path> CPrint::GetDirectoryLocalPaths(const std::filesystem::path& searchPath) const
{
	std::vector<fs::path> paths;
	for (const auto& item : fs::directory_iterator(searchPath))
	{
		paths.push_back(item);
	}
	return paths;
}
