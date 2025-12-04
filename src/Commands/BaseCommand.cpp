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
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& pattern = m_v_CommandDispatch[i].first;
		
		std::vector<std::string> UserArgs;
		uint8_t ParamFlag;
		SearchSuccess = ParseCommand(pattern, UserArgs, ParamFlag);
		
		if (SearchSuccess)
		{	
			const bool ExecuteSuccess = m_v_CommandDispatch[i].second(UserArgs, ParamFlag);

			return ExecuteSuccess;
		}
	}
	if (!SearchSuccess)
		ReportInvalidCommand();

	return SearchSuccess;
}

bool BaseCommand::ParseCommand(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs, uint8_t& paramFlag)
{
	size_t Start;
	size_t End;
	const std::vector<std::string> ParamFlags = ExtractParamFlags(Start, End);
	paramFlag = GetParamFlagsAsFlag(ParamFlags);

	if (Start >= m_v_args.size())
		return false;

	for (size_t i = 0; i < Start; i++)
	{
		if (pattern[i] != m_v_args[i])
			return false;
	}

	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == ARG_USER_INP)
		{
			userArgs.push_back(m_v_args[Start + i]);
		}
		else if (pattern[i] == ARG_MULTI_INP)
		{
			std::copy(m_v_args.begin() + End, m_v_args.end(), std::back_inserter(userArgs));
			break;
		}
	}

	return true;
}
std::vector<std::string> BaseCommand::ExtractParamFlags(size_t& start, size_t& end)
{
	std::vector<std::string> extractedFlags;
	start = 0;
	end = 0;

	for (size_t i = 0; i < m_v_args.size(); i++)
	{
		if (m_v_args[i][0] == '-')
		{
			start = i;
			extractedFlags.push_back(m_v_args[i]);
			break;
		}
	}
	end = start;
	for (size_t j = start; j < m_v_args.size(); j++, end++)
	{
		if (m_v_args[j][0] == '-')
		{
			if(std::find(extractedFlags.begin(), extractedFlags.end(), m_v_args[j]) == extractedFlags.end())
				extractedFlags.push_back(m_v_args[j]);
		}
		else
		{
			break;
		}
		
	}

	return extractedFlags;
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
bool BaseCommand::CheckParemetersFound(const std::vector<std::string>& args, const std::string& commandHandle) const
{
	if (args.empty())
	{
		OutputLog::Get().ReportStatus("No valid paremeter(s) for: " + commandHandle + " found.", MessageType::EERROR);
		return false;
	}
	return true;
}