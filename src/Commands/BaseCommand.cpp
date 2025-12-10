#include "PCH.hpp"
#include "Commands/BaseCommand.hpp"
#include "OutputLog.hpp"
#include "FSCLT.hpp"


void BaseCommand::BindCommand(const std::vector<std::string>& pattern, std::function<bool(const std::vector<std::string>& userArg, uint8_t paramFlag)> func)
{
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;
		
		assert(!(strVec == pattern) && "You cant bind a command with the same pattern");
	}
	m_v_CommandDispatch.push_back(std::make_pair(pattern, func));
}
bool BaseCommand::Execute()
{
	bool SearchSuccess = false;
	
	std::vector<std::string> UserArgs;
	uint8_t ParamFlag;
	auto test = TryParseCommand(UserArgs, ParamFlag);
		
	if (SearchSuccess)
	{	
		const bool ExecuteSuccess = m_v_CommandDispatch[i].second(UserArgs, ParamFlag);

		return ExecuteSuccess;
	}

	if (!SearchSuccess)
		ReportInvalidCommand();

	return SearchSuccess;
}

std::vector<std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs, uint8_t ParamFlags)>>>

BaseCommand::TryParseCommand(std::vector<std::string>& userArgs, uint8_t& paramFlag)
{
	/*std::vector<std::vector<std::string>> matchingPatterns = MatchUserArgumentsWithPattern();

	for (auto it = matchingPatterns.begin(); it != matchingPatterns.end(); )
	{
		size_t firstParamLocation = GetUserParamPosition(*it);
	
		for (size_t i = 0; i < firstParamLocation; i++)
		{
			if (i >= m_v_args.size() || m_v_args[i] != it->at(i))
			{
				it = matchingPatterns.erase(it);
				break;
			}
			
		}
		if (it == matchingPatterns.end())
			break;
		else
			it++;
	}

*/

	return matchingPatterns;
}
std::vector<std::vector<std::string>> BaseCommand::MatchUserArgumentsWithPattern()
{
	std::vector<std::vector<std::string>> MatchingPatterns;
	for (const auto& item : m_v_CommandDispatch)
	{
		MatchingPatterns.push_back(item.first);
	}
	
	/*
		We want to go through all command patterns and sort all patterns out which are not equal to the users arguments.
		We first go through all command patterns and check the 0 string in m_v_args and 0 string in pattern.
		We reapeat it until checkLocation is bigger then the arguments in m_v_args
	*/

	size_t checkLocation = 0;
	while (!(checkLocation >= m_v_args.size()))
	{
		for (auto it = MatchingPatterns.begin(); it != MatchingPatterns.end();)
		{
			if (m_v_args[checkLocation] != it->at(checkLocation))
			{
				it = MatchingPatterns.erase(it);
			}
			else
			{
				it++;
			}
		}
		checkLocation++;
	}

	return MatchingPatterns;
}
std::vector<std::string> BaseCommand::ExtractParamFlags(size_t& start, size_t& end)
{
	std::vector<std::string> extractedFlags;


	return extractedFlags;
}
size_t BaseCommand::GetUserParamPosition(const std::vector<std::string>& pattern) const
{
	size_t i = 0;
	for (const auto& item : pattern)
	{
		if (
			item == ARG_USER_INP ||
			item == ARG_PARAM_FLAGS ||
			item == ARG_MULTI_INP
			)
		{
			return i;
		}
		i++;
	}

	return 0;
}
void BaseCommand::ReportInvalidCommand()
{
	std::string errorString = "Couldn't find Command: " + m_CommandName + " ";

	for (const auto& item : m_v_args)
		errorString.append(item + " ");

	OutputLog::Get().ReportStatus(errorString, MessageType::EERROR);
}
uint8_t BaseCommand::GetParamFlagsAsFlag(const std::vector<std::string>& flagsVec) const
{
	uint8_t flags = EFLAG_PARAM::ENONE;

	for (const auto& item : flagsVec)
	{
		auto it = m_um_Flags.find(item);
		if (it != m_um_Flags.end())
		{
			flags &= ~EFLAG_PARAM::ENONE;
			flags |= it->second;
		}
		else
		{
			OutputLog::Get().ReportStatus("Invalid parameter flag: [" + item + "]", MessageType::EERROR);
			break;
		}
	}
	
	return flags;
}