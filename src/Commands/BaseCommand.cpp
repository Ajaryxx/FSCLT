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
	size_t i = 0;

	for (const auto& item : pattern)
	{
		/*if (i >= m_v_args.size())
			return false;*/

		if (item == ARG_PARAM_FLAGS)
		{
			size_t jumpedOver;
			std::vector<std::string> paramFlags = ExtractParamFlags(i, jumpedOver);
			if (paramFlags.empty())
				continue;

			paramFlag = GetParamFlagsAsFlag(paramFlags);
			i += jumpedOver - 1;

		}
		else if (item == ARG_USER_INP)
		{
			userArgs.push_back(item);
		}
		else if (item == ARG_MULTI_INP)
		{
			std::copy(m_v_args.cbegin() + i, m_v_args.cend(), std::back_inserter(userArgs));
			break;
		}
		else if(item != m_v_args[i])
		{
			return false;
		}
		i++;
	}
	
	return true;
}
std::vector<std::string> BaseCommand::ExtractParamFlags(size_t offset, size_t& jumpedOver)
{
	std::vector<std::string> extractedFlags;

	size_t ParseJump = 0;
	if (m_v_args[offset][0] != '-')
		return extractedFlags;

	for (auto it = m_v_args.cbegin() + offset; it != m_v_args.cend(); it++)
	{
		if (it->at(0) == '-')
		{
			if(std::find(extractedFlags.begin(), extractedFlags.end(), *it) == extractedFlags.end())
				extractedFlags.push_back(*it);

			ParseJump++;
		}
		else
		{
			break;
		}
	}
	jumpedOver = ParseJump;

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