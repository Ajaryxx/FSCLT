#include "PCH.hpp"
#include "CommandModules/CSearch.hpp"
#include "OutputLog.hpp"
#include "FilesystemUtilityHelper.hpp"
#include "FSCLT.hpp"

namespace fs = std::filesystem;


CSearch::CSearch(const std::vector<std::string>& args) : BaseCommandModule(CSEARCH_NAME, args)
{
	//searches for an element in the filesystem
	BIND_COMMAND(std::vector<std::string>({"element", ARG_PARAM_FLAGS, ARG_MULTI_INP}), CSearch, HandleSearchElement);

	//searches for a word in a file in the filesystem
	BIND_COMMAND(std::vector<std::string>({ "element", "word", ARG_PARAM_FLAGS, ARG_MULTI_INP }), CSearch, HandleSearchWordInFile);
}
void CSearch::PrintUsageInfo() const
{

}
bool CSearch::HandleSearchElement(const std::vector<std::string>& UserArgs, uint8_t flags)
{

	return true;
}
bool CSearch::HandleSearchWordInFile(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	return true;
}