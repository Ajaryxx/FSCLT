#include "PCH.hpp"
#include "Commands/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

CPrint::CPrint(const std::vector<std::string>& args) : BaseCommand(CMD_NAME, args)
{
	BIND_COMMAND(std::vector<std::string>({ "info", "version" }), HandlePrintVersion);
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_MULTIINP }), HandlePrintCommands);

	BIND_COMMAND(std::vector<std::string>({ "info", "dir", ARG_MULTIINP }), HandlePrintInfoDirectory);
	BIND_COMMAND(std::vector<std::string>({ "info", "file", ARG_MULTIINP }), HandlePrintInfoFile);

	BIND_COMMAND(std::vector<std::string>({ "list", "dir", ARG_MULTIINP }), HandlePrintListDirectorys);
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
	OutputLog::Get().SendMessage(TOOL_VERSION, 1);
	
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
				OutputLog::Get().ReportStatus("Couldn't find Command: " + item, MessageType::EERROR);
				return false;
			}
		}
	}
	return true;
}
bool CPrint::HandlePrintListDirectorys(const std::vector<std::string>& UserArgs)
{
	std::vector<fs::path> buffer;
	std::string ExecutePath = FSCLT::Get().GetExecutePath();
	ExecutePath = "C:\\Users\\joelf\\Documents";

	OutputLog& log = OutputLog::Get();
	log.ReportStatus("There are following directorys: ");
	log.Seperate();

	bool succses = true;
	try
	{	//print all dirs
		if (UserArgs.empty())
		{
			for (const auto& item : fs::directory_iterator(ExecutePath))
			{
				if (item.is_directory())
					buffer.push_back(item);
			}
		}
		else
		{

			for (const auto& arg : UserArgs)
			{
				const fs::path dir = DoesExists(arg, ExecutePath);
				if (!dir.empty())
					buffer.push_back(dir);
				else
					log.ReportStatus("Couldn't find directory with name: \"" + arg + "\" in directory: " + ExecutePath, MessageType::WARNING, 1);
			}
		}
		
	}
	catch (const fs::filesystem_error& err)
	{
		log.ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR);
		succses = false;
	}

	if (buffer.empty())
		log.SendMessage("No directories found", 0, Color::CYAN);
	else
		log.PrintDirInfo(buffer);

	return succses;
}
std::filesystem::path CPrint::DoesExists(const std::string name, const std::filesystem::path& fullPath) const
{
	for (const auto& dir : fs::directory_iterator(fullPath))
	{
		fs::path dirPath = dir.path().parent_path().append(name);
		if (fs::exists(dirPath))
		{
			return dirPath;
		}
	}
	return fs::path();
}

bool CPrint::HandlePrintListFiles(const std::vector<std::string>& UserArgs)
{
	std::vector<fs::path> buffer;
	std::string ExecutePath = FSCLT::Get().GetExecutePath();
	ExecutePath = "C:\\Users\\joelf\\Pictures";

	OutputLog& log = OutputLog::Get();
	log.ReportStatus("There are following files in this directory: ");
	log.Seperate();

	bool succses = true;
	try
	{
		//print all files
		if (UserArgs.empty())
		{
			for (const auto& item : fs::directory_iterator(ExecutePath))
			{
				if (item.is_regular_file())
					buffer.push_back(item);
			}
		}
		else
		{
			//print specific files
			for (const auto& arg : UserArgs)
			{
				std::vector<fs::path> foundFiles = CheckFileExtInDir(ExecutePath, arg);
				std::copy(foundFiles.begin(), foundFiles.end(), std::back_inserter(buffer));
			
			}
		}

	}
	catch (const fs::filesystem_error& err)
	{
		log.ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR);
		succses = false;
	}

	if (buffer.empty())
		log.SendMessage("No files found", 1, Color::CYAN);
	else
		log.PrintFileInfo(buffer);

	return succses;
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