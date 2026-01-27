#include "PCH.hpp"
#include "CommandModules/CPrint.hpp"
#include "FSCLT.hpp"
#include "Utility/OutputLog.hpp"
#include "Utility/FilesystemUtilityHelper.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

CPrint::CPrint() : BaseCommandModule(MODULE_NAME)
{
	DECLARE_SUB_COMMAND("info version {TEST} {PARTY}", CPrint, HandlePrintOutVersion);
	

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

