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
			bool ExecuteSuccess = m_v_CommandDispatch[i].second(UserArgs, ParamFlag);

			return ExecuteSuccess;
		}
	}
	if (!SearchSuccess)
		ReportInvalidCommand();

	return SearchSuccess;
}

bool BaseCommand::ParseCommand(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs, uint8_t& paramFlag)
{
	size_t i;
	for (i = 0; i < pattern.size(); i++)
	{
		//Check so we dont get an out of bounds error
		if (i >= m_v_args.size())
			return true;

		if (pattern[i] == ARG_PARAM_FLAGS)
		{
			std::vector<std::string> ParamFlags = ExtractParamFlags(std::vector<std::string>(m_v_args.begin() + i, m_v_args.end()));
			paramFlag = GetParamFlagsAsFlag(ParamFlags);
		}
		else if (pattern[i] == ARG_USER_INP)
		{
			//Store user argument to the user args vector
			userArgs.push_back(m_v_args[i]);
		}
		else if (pattern[i] == ARG_MULTI_INP)
		{
			if (i >= m_v_args.size())
				break;

			//get all args until end of vec
			userArgs.insert(userArgs.end(), m_v_args.begin() + i, m_v_args.end());
			i += m_v_args.size();
			break;
		}
		//We have to check this at the end
		else if (pattern[i] != m_v_args[i])
		{
			return false;
		}
	}
	
	return true;
}
void BaseCommand::ReportInvalidCommand()
{
	std::string errorString = "Couldn't find Command: " + m_CommandName + " ";

	for (const auto& item : m_v_args)
		errorString.append(item + " ");

	OutputLog::Get().ReportStatus(errorString, MessageType::EERROR);
}
std::vector<std::string> BaseCommand::ExtractParamFlags(const std::vector<std::string>& userParams)
{
	std::vector<std::string> v_paramFlags;

	if (userParams.empty())
		return v_paramFlags;

	if (userParams[0][0] == '-')
	{
		for (const auto& item : userParams)
		{
			if (item[0] != '-')
			{
				break;
			}
			else
			{
				if(std::find(v_paramFlags.begin(), v_paramFlags.end(), item) == v_paramFlags.end())
				v_paramFlags.push_back(item);
			}
		}
	}
	return v_paramFlags;
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