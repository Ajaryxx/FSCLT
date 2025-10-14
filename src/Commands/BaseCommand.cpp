#include "Commands/BaseCommand.hpp"


void BaseCommand::PrintError()
{
}
void BaseCommand::BindCommand(const std::vector<std::string>& pattern, std::function<void()> func)
{
	
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;
		
		assert(!(strVec == pattern) && "You cant bind a command with the same pattern");
			
	}

	m_v_CommandDispatch.push_back(std::make_pair(pattern, func));
	
}
void BaseCommand::Execute()
{
	for (size_t i = 0; i < m_v_CommandDispatch.size(); i++)
	{
		const std::vector<std::string>& strVec = m_v_CommandDispatch[i].first;

		if(strVec == m_v_args)

	}

}
