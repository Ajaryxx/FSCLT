#include "Commands/BaseCommand.hpp"


void BaseCommand::PrintError()
{
	
}
void BaseCommand::BindCommand(const std::vector<std::string>& pattern, std::function<void(const std::vector<std::string>& userArg)> func)
{
	
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;
		
		assert(!(strVec == pattern) && "You cant bind a command with the same pattern");
			
	}

	m_v_CommandDispatch.push_back(std::make_pair(pattern, func));
	
}
void BaseCommand::Execute(const std::vector<std::string>& userArg)
{
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;
		std::vector<std::string> UserArgs;

		if (strVec.size() != m_v_args.size())
			continue;

		//Check for equal a command
		bool IsEqual = true;
		for (size_t j = 0; j < strVec.size(); j++)
		{
			if (strVec[j] == ARG_USERINP)
			{
				UserArgs.push_back(m_v_args[j]);
			}
			else if (strVec[j] != m_v_args[j])
			{
				IsEqual = false;
			}
				
		}
		if (IsEqual)
		{
			std::cout << "Running Command [" << m_CommandName << "]..." << std::endl;
			m_v_CommandDispatch[i].second(UserArgs);
			return;
		}
	}
	std::cerr << "Invalid Command [" << m_CommandName << "] ";
	for (const auto& item : m_v_args)
		std::cerr << "[" << item << "] ";
}
