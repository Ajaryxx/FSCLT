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
	bool success = false;
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;
		std::vector<std::string> UserArgs;

		if (strVec.size() != m_v_args.size())
			continue;

		//Check for an equal command
		bool IsEqual = true;
		for (size_t j = 0; j < strVec.size(); j++)
		{
			if (strVec[j] == ARG_USERINP)
			{
				//Store user argument to the user args vector
				UserArgs.push_back(m_v_args[j]);
			}
			else if (strVec[j] != m_v_args[j])
			{
				IsEqual = false;
			}
				
		}
		if (IsEqual)
		{
			FSCLT::Get().ReportMessage("Running Command: [" + m_CommandName + "]...", MessageType::INFO);
			//returns if the command operation was succsessful
			success = m_v_CommandDispatch[i].second(UserArgs);
			FSCLT::Get().ReportMessage("Command: [" + m_CommandName + "] successful executed.", MessageType::INFO);
			FSCLT::Get().MakeNewLine(3);
			return success;
		}
	}
	std::string errorString;
	
	for (const auto& item : m_v_args)
		errorString.append(item + " ");

	FSCLT::Get().ReportMessage(errorString, MessageType::ERROR);

	return success;
}
