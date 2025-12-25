#include "PCH.hpp"
#include "CommandModules/BaseCommandModule.hpp"
#include "OutputLog.hpp"
#include "FSCLT.hpp"


void BaseCommandModule::BindCommand(const std::vector<std::string>& pattern, std::function<bool(const std::vector<std::string>& userArg, uint8_t paramFlag)> func)
{
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;
		
		assert(!(strVec == pattern) && "You cant bind a command with the same pattern");
	}
	m_v_CommandDispatch.push_back(std::make_pair(pattern, func));
}
bool BaseCommandModule::Execute()
{
	std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs, uint8_t ParamFlags)>> cmd;
	std::vector<std::string> UserArgs;
	uint8_t ParamFlag;
	bool SearchSuccess = TryParseUserCommand(cmd, UserArgs, ParamFlag);

	if (SearchSuccess)
	{
		const bool ExecuteSuccess = cmd.second(UserArgs, ParamFlag);
		return ExecuteSuccess;
	}
	else
	{
		ReportInvalidCommand();
	}
	
	return SearchSuccess;

}
bool BaseCommandModule::IsEqualWithPattern(const std::vector<std::string>& pattern, std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs, uint8_t ParamFlags)>> element)
{
	return element.first == pattern;
}

bool BaseCommandModule::TryParseUserCommand(std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs, uint8_t ParamFlags)>>& cmd, 
	std::vector<std::string>& userArgs, uint8_t& paramFlags)
{
	auto matchingPatterns = GetMatchingPatterns();
	std::vector<std::string> test = GetCloseMatchingPattern(matchingPatterns);
	if (test.empty())
		return false;

			GetUserArgumentsWithPattern(test, userArgs, paramFlags);
			auto it = std::ranges::find(m_v_CommandDispatch, test, &std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>&userArgs, uint8_t ParamFlags)>>::first);
			if (it != m_v_CommandDispatch.end())
			{
				cmd = *it;
				return true;
			}
			else
			{
				return false;
			}
	
	return false;
}
std::vector<std::string> BaseCommandModule::GetCloseMatchingPattern(const std::vector<std::vector<std::string>>& vec)
{
	std::multimap<int, std::vector<std::string>> map;
	
	size_t i;
	for (const auto& item : vec)
	{
		bool noMatching = true;
		for (i = 0; i < m_v_args.size(); i++)
		{
			
			if(i >= item.size() || m_v_args[i] != item[i])
			{
				if (map.find(i) == map.end())
				{
					noMatching = false;
					map.insert({ i, item });
					break;
				}
				else
				{
					return std::vector<std::string>();
				}
			}
		}
		if(noMatching)
			map.insert({ i, item });
	}
	
	if (map.rbegin() != map.rend())
	{
		return map.rbegin()->second;
	}
	return std::vector<std::string>();
}
void BaseCommandModule::GetUserArgumentsWithPattern(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs, uint8_t& paramFlags)
{
	size_t offset = 0;
	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == ARG_PARAM_FLAGS)
		{
			std::vector<std::string> flags = ExtractParamFlags();

			paramFlags = GetParamFlagsAsFlag(flags);

			offset += flags.size();
		}
		else if (pattern[i] == ARG_USER_INP)
		{
			userArgs.push_back(m_v_args[i + offset]);
		}
		else if (pattern[i] == ARG_MULTI_INP)
		{
			if (i + offset > m_v_args.size())
				break;
			std::copy(m_v_args.begin() + i, m_v_args.end(), std::back_inserter(userArgs));
			break;
		}

	}
}
std::vector<std::string> BaseCommandModule::ExtractParamFlags()
{
	std::vector<std::string> extractedFlags;

	for (const auto& item : m_v_args)
	{
		bool FoundFirstFlag = false;
		if (item[0] == '-')
		{
			FoundFirstFlag = true;
			extractedFlags.push_back(item);
		}
		else
		{
			if(FoundFirstFlag)
				break;
		}
	}

	return extractedFlags;
}
std::vector<std::vector<std::string>> BaseCommandModule::GetMatchingPatterns()
{	
	std::vector<std::vector<std::string>> matchingPatterns;

	for (const auto& item : m_v_CommandDispatch)
	{
		bool succsesMatching = true;
		std::vector<std::string> patt = item.first;
		for (size_t i = 0; i < patt.size(); i++)
		{
			if (i >= m_v_args.size() || !IsParameter(patt, i) && m_v_args[i] != patt[i])
			{
				succsesMatching = false;
				break;
			}
			if(IsParameter(patt, i+1))
				break;
		}
		if (succsesMatching)
			matchingPatterns.push_back(patt);
	}

	return matchingPatterns;
}
bool BaseCommandModule::IsParameter(const std::vector<std::string>& pattern, size_t location) const
{
	if (location >= pattern.size())
		return false;

	return (
		pattern[location] == ARG_USER_INP ||
		pattern[location] == ARG_PARAM_FLAGS ||
		pattern[location] == ARG_MULTI_INP
		);
}
size_t BaseCommandModule::GetFirstUserParamPosition(const std::vector<std::string>& pattern) const
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
void BaseCommandModule::ReportInvalidCommand()
{
	std::string errorString = "Couldn't find Command: " + m_CommandName + " ";

	for (const auto& item : m_v_args)
		errorString.append(item + " ");

	OutputLog::Get().ReportStatus(errorString, MessageType::EERROR);
}
uint8_t BaseCommandModule::GetParamFlagsAsFlag(const std::vector<std::string>& flagsVec) const
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