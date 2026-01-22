#include "PCH.hpp"
#include "FSCLT.hpp"
#include "CommandModules/Commands.hpp"
#include "Utility/OutputLog.hpp"

FSCLT* FSCLT::fsclt = nullptr;


FSCLT::FSCLT(int argc, const std::vector<std::string>& argv)
{
	this->m_Argc = argc;
	this->m_Argv = argv;
	fsclt = this;

	InitzializeCommands();
}
FSCLT::~FSCLT()
{
	if (!m_v_Commands.empty())
	{
		for (auto& item : m_v_Commands)
		{
			delete item;
			item = nullptr;
		}
		m_v_Commands.clear();
	}
}
bool FSCLT::Run()
{
	if (m_Argc == 1)
	{
		OutputLog::Get().ReportStatus("Usage: fsclt [Command Flag] [ARG1] [ARG2] AND [Command Flag] [ARG1] [ARG2] | ...\nFor example: fsclt print info all");
		return false;
	}
	bool succes = ParseCommandLine();

	if (!succes)
		return false;

	succes = ExecuteCommands();

	return succes;
}

bool FSCLT::ExecuteCommands()
{
	for (const auto& item : m_v_Commands)
	{
		OutputLog::Get().ReportStatus("Trying to execute: " + item->GetCommandModuleName() + "...", MessageType::INFO, 1);

		if (!item->Execute())
		{
			OutputLog::Get().ReportStatus("Failed to execute: " + item->GetCommandModuleName(), MessageType::EERROR, 1);
			return false;
			
		}
		OutputLog::Get().ReportStatus("Execution successful: " + item->GetCommandModuleName(), MessageType::INFO, 1);
	}

	return true;
}

bool FSCLT::ParseCommandLine()
{
	//Jump over path to exe
	for (size_t i = 1; i < m_Argc; i++)
	{
		//does prefix exists already?
		auto it = m_um_CommandFlags.find(m_Argv[i]);

		if (it != m_um_CommandFlags.end())
		{
			size_t newOffset;
			
			std::vector<std::string> args = CatchArguments(i + 1, newOffset, ';');

			//Stores the Command to the vector buffer so we can execute it later
			it->second->InitializeArguments(args);
			m_v_Commands.push_back(it->second);

			//jump over all arguments of this current Command
			i += args.size();
		}
		else
		{
			OutputLog::Get().ReportStatus("Couldn't find Command: [" + m_Argv[i] + "]\nCommands wont be executed!", MessageType::EERROR);
			
			return false;
		}
	}
	return true;
}
std::vector<std::string> FSCLT::CatchArguments(size_t offset, size_t& newOffset, char StopPrefix)
{
	std::vector<std::string> args;
	for (size_t i = offset; i < m_Argc; i++)
	{
		if (m_Argv[i][0] != StopPrefix)
		{
			args.push_back(m_Argv[i]);
		}
		else
			break;
	}
	//to jump over the prefix
	newOffset = args.size() + 1;

	return args;
}

void FSCLT::InitzializeCommands()
{
	//Print useful information like version or command info
	DECLARE_COMMAND_FLAG("print", CPrint);


	
}
