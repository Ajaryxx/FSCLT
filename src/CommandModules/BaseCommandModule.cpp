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
	return true;
}

void BaseCommandModule::InitializeArguments(const std::vector<std::string>& args)
{
	this->m_v_args = args;
}

void BaseCommandModule::ReportInvalidCommand()
{
	std::string errorString = "Couldn't find Command: " + m_CommandName + " ";

	for (const auto& item : m_v_args)
		errorString.append(item + " ");

	OutputLog::Get().ReportStatus(errorString, MessageType::EERROR);
}
