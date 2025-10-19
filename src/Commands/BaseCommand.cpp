#include "PCH.hpp"
#include "Commands/BaseCommand.hpp"
#include "FSCLT.hpp"


void BaseCommand::BindCommand(const std::vector<std::string>& pattern, std::function<bool(const std::vector<std::string>& userArg)> func)
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
		SearchSuccess = CheckEqualCommand(pattern, UserArgs);
		
		if (SearchSuccess)
		{	
			bool ExecuteSuccess = m_v_CommandDispatch[i].second(UserArgs);

			return ExecuteSuccess;
		}
	}
	if (!SearchSuccess)
		ReportInvalidCommand();

	return SearchSuccess;
}

bool BaseCommand::CheckEqualCommand(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs)
{
	std::vector<std::string> UserArgs;
	size_t i;
	for (i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == ARG_MULTIINP)
		{
			UserArgs.insert(UserArgs.end(), m_v_args.begin() + i, m_v_args.end());
			i += m_v_args.size();
			break;
		}
		else if (pattern[i] == ARG_USERINP)
		{
			//Store user argument to the user args vector
			UserArgs.push_back(m_v_args[i]);
		}
		else if (pattern[i] != m_v_args[i])
		{
			return false;
		}
	}
	
	userArgs = UserArgs;
	return i >= m_v_args.size();
}
void BaseCommand::ReportInvalidCommand()
{
	std::string errorString = "Couldn't find Command: " + m_CommandName + " ";

	for (const auto& item : m_v_args)
		errorString.append(item + " ");

	FSCLT::Get().ReportMessage(errorString, MessageType::ERROR);
}