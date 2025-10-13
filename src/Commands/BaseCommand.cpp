#include "Commands/BaseCommand.hpp"


void BaseCommand::PrintError()
{
}
void BaseCommand::BindCommand(const std::string& name, std::vector<std::string> pattern, std::function<void()> func)
{
	auto& it = m_um_CommandDispatch.find(pattern);
	if (it != m_um_CommandDispatch.end())
	{
		std::cerr << "Command with pattern: [ ";
		for (const auto& item : pattern)
		{
			std::cerr <<  item << ", ";
		}
		std::cerr << "] is already bound\n";
		return;
	}
	m_um_CommandDispatch[pattern] = func;
	
}
void BaseCommand::Execute()
{

}
