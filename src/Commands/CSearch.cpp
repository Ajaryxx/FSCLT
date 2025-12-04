#include "PCH.hpp"
#include "Commands/CSearch.hpp"
#include "OutputLog.hpp"
#include "FilesystemUtilityHelper.hpp"
#include "FSCLT.hpp"

namespace fs = std::filesystem;


CSearch::CSearch(const std::vector<std::string>& args) : BaseCommand(CSEARCH_NAME, args)
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
	if (!CheckParemetersFound(UserArgs, "HANDLE_SEARCH_ELEMENT"))
		return false;

	OutputLog& log = OutputLog::Get();
	FilesystemUtilityHelper& helper = FilesystemUtilityHelper::Get();

	if (flags & EFLAG_PARAM::EFLAG_LOC && flags & EFLAG_PARAM::EFLAG_RECURSIVE)
	{
		log.ReportStatus("You can not use both flags: [-loc] and [-r].", MessageType::EERROR);

		return false;
	}
	std::unordered_multimap<std::string, std::string> paths;
	std::vector<fs::path> path;
	switch (flags)
	{
	case EFLAG_PARAM::EFLAG_LOC: path = helper.GetDirectoryLocalPaths(FSCLT::Get().GetExecutePath()); break;

	case EFLAG_PARAM::EFLAG_RECURSIVE: path = helper.GetDirectoryRecursivePaths(FSCLT::Get().GetExecutePath()); break;
	default:
		log.ReportStatus("No valid flag was found", MessageType::EERROR);
		return false;
		break;
	}

	//make key values
	for (const auto& item : path)
		paths.emplace(item.stem().string(), item.string());

	//search for equal name
	for (const auto& arg : UserArgs)
	{
		std::vector<std::string> foundElements;
		auto range = paths.equal_range(arg);
		for (auto& it = range.first; it != range.second; it++)
		{
			foundElements.push_back(it->second);
		}

		log.PrintGroup(arg, foundElements);
	}
	
	return true;
}
bool CSearch::HandleSearchWordInFile(const std::vector<std::string>& UserArgs, uint8_t flags)
{
	if (!CheckParemetersFound(UserArgs, "HANDLE_SEARCH_WORD_IN_ELEMENT"))
		return false;

	OutputLog& log = OutputLog::Get();
	FilesystemUtilityHelper& helper = FilesystemUtilityHelper::Get();

	if (flags & EFLAG_PARAM::EFLAG_LOC && flags & EFLAG_PARAM::EFLAG_RECURSIVE)
	{
		log.ReportStatus("You can not use both flags: [-loc] and [-r].", MessageType::EERROR);

		return false;
	}
	std::unordered_multimap<std::string, std::string> paths;
	std::vector<fs::path> path;

	switch (flags)
	{
	case EFLAG_PARAM::EFLAG_LOC: path = helper.GetDirectoryLocalPaths(FSCLT::Get().GetExecutePath()); break;

	case EFLAG_PARAM::EFLAG_RECURSIVE: path = helper.GetDirectoryRecursivePaths(FSCLT::Get().GetExecutePath()); break;
	default:
		log.ReportStatus("No valid flag was found", MessageType::EERROR);
		return false;
		break;
	}

}