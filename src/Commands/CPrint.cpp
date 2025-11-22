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

	if (UserArgs.size() < 1)
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
	log.SetSpace(1);
	if (UserArgs.empty())
	{
		path = GetDirectoryLocalPaths(FSCLT::Get().GetExecutePath());
		std::vector<std::string> stringPaths(path.size());
		
		for (const auto& item : path)
			std::transform(path.begin(), path.end(), stringPaths.begin(), [&](const fs::path& str) { return str.string(); });

		log.PrintList(stringPaths, "Directory List", Color::MAGENTA);
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
