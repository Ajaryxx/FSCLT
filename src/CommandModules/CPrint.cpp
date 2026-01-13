#include "PCH.hpp"
#include "CommandModules/CPrint.hpp"
#include "FSCLT.hpp"
#include "OutputLog.hpp"
#include "FilesystemUtilityHelper.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

CPrint::CPrint() : BaseCommandModule(CMD_NAME)
{
	//prints out the current Tool version
	BIND_COMMAND(std::vector<std::string>({ "info", "version"}), CPrint, HandlePrintOutVersion);

	//prints out the usage from command(s)
	BIND_COMMAND(std::vector<std::string>({ "info", "command", ARG_PARAM_FLAGS, ARG_MULTI_INP}), CPrint, HandlePrintListDirectory);

	//prints out the usage from command(s)
	BIND_COMMAND(std::vector<std::string>({ "info", "command", "dir", ARG_MULTI_INP}), CPrint, HandlePrintOutCommandInfo);

	//prints out the directory list
	BIND_COMMAND(std::vector<std::string>({ "list", "dir", ARG_MULTI_INP}), CPrint, HandlePrintListDirectory);

	//prints out the info of this/these element(s)
	BIND_COMMAND(std::vector<std::string>({ "info", "element", ARG_MULTI_INP}), CPrint, HandlePrintInfoElement);
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
	OutputLog::Get().SendOutput("buz buz");
	return true;
}

bool CPrint::HandlePrintListDirectory(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	OutputLog& log = OutputLog::Get();
	FilesystemUtilityHelper& utilityHelper = FilesystemUtilityHelper::Get();

	OutputLog::Get().SendOutput("biz biz");
	return true;
}

bool CPrint::HandlePrintInfoElement(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	return true;
}

