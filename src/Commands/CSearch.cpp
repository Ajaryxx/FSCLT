#include "PCH.hpp"
#include "Commands/CSearch.hpp"
#include "OutputLog.hpp"

namespace fs = std::filesystem;


CSearch::CSearch(const std::vector<std::string>& args) : BaseCommand(CSEARCH_NAME, args)
{
	//searches for an element in the filesystem
	BIND_COMMAND(std::vector<std::string>({"element", ARG_PARAM_FLAGS, ARG_MULTI_INP}), CSearch, HandleSearchElement);
}
void CSearch::PrintUsageInfo() const
{

}
bool CSearch::HandleSearchElement(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	if (!CheckParemetersFound(UserArgs, "HANDLE_SEARCH_ELEMENT"))
		return false;

	OutputLog& log = OutputLog::Get();

	if (flags & EFLAG_PARAM::EFLAG_LOC && flags & EFLAG_PARAM::EFLAG_RECURSIVE)
	{
		log.ReportStatus("You can not use both flags: [-loc] and [-r].", MessageType::EERROR);

		return false;
	}
	std::vector<fs::path> paths;
	switch (flags)
	{
	case EFLAG_PARAM::EFLAG_LOC:
		
		break;

	case EFLAG_PARAM::EFLAG_RECURSIVE:

		break;


	default:
		log.ReportStatus("No valid flag was found", MessageType::EERROR);
		return false;
		break;
	}

	return true;
}