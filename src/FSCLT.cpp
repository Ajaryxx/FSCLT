#include "PCH.hpp"
#include "FSCLT.hpp"
#include "Commands/Commands.hpp"

FSCLT* FSCLT::fsclt = nullptr;

FSCLT::FSCLT(int argc, const std::vector<std::string>& argv)
{
	this->m_Argc = argc;
	this->m_Argv = argv;
	fsclt = this;

	InitzializeCommands();
}

bool FSCLT::Run()
{
	if (m_Argc == 1)
	{
		std::cerr << "Usage: fsclt [Command Flag] [ARG1] [ARG2] | [Command Flag] [ARG1] [ARG2] | ...\nFor example: fsclt print info all";
		return false;
	}
	bool CanExecuteAll = Parse();

	if (!CanExecuteAll)
		return false;

	for (const auto& item : m_v_Commands)
	{
		item->Execute();
		
		for (auto& item : m_v_TempCommandBuffer)
		{
			delete item;
			item = nullptr;
		}
		m_v_TempCommandBuffer.clear();
	}
}
bool FSCLT::Parse()
{
	//Jump over path to exe
	for (size_t i = 1; i < m_Argc; i++)
	{
		auto& it = m_um_CommandFlags.find(m_Argv[i]);

		if (it != m_um_CommandFlags.end())
		{
			size_t newOffset;
			//Jump over current string(Command string)
			std::vector<std::string>& buffer = CatchArguments(i + 1, newOffset);
			//Stores the Command to the vector buffer so we can execute it later
			it->second(true, buffer);
			i += newOffset;
		}
		else
		{
			std::cerr << "Couldn't find Command: [" << m_Argv[i] << "]\n";
			return false;
		}
	}
	return true;
}
std::vector<std::string> FSCLT::CatchArguments(size_t offset, size_t& newOffset)
{
	std::vector<std::string> args;
	for (size_t i = offset; i < m_Argc; i++)
	{
		if (m_Argv[i] != "|")
		{
			args.push_back(m_Argv[i]);
		}
		else
			break;
	}

	//to jump over '|'
	newOffset = args.size() + 1;
	return args;
}
const std::vector<BaseCommand*>& FSCLT::GetAllCommands() const
{
	for (auto it = m_um_CommandFlags.begin(); it != m_um_CommandFlags.end(); it++)
	{
		it->second(false, std::vector<std::string>());
	}

	return m_v_TempCommandBuffer;
}
BaseCommand* FSCLT::GetCommand(const std::string& name) const
{
	auto& it = m_um_CommandFlags.find(name);

	if (it != m_um_CommandFlags.end())
	{
		BaseCommand* cmd = it->second(false, std::vector<std::string>());
		return cmd;
	}

	return nullptr;
}
FSCLT& FSCLT::Get()
{
	return *fsclt;
}
void FSCLT::InitzializeCommands()
{
	//Print useful information like version or commands
	DECLARE_COMMAND_FLAG("print", CPrint);
}
