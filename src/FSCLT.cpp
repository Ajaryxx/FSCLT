#include "PCH.hpp"
#include "FSCLT.hpp"
#include "Commands/CVersion.hpp"

FSCLT::FSCLT(int argc, char* argv[])
{
	this->m_argc = argc;
	this->m_argv = argv;
}

bool FSCLT::Run()
{
	if (m_argc == 1)
	{
		std::cerr << "Usage: fsclt [Command Flag] [ARG1] [ARG2] | [Command Flag] [ARG1] [ARG2] | ...\nFor example: fsclt print info all";
		return false;
	}
	Parse();
	
	for (const auto& item : m_v_Commands)
	{
		item->Execute();
	}
}
bool FSCLT::Parse()
{
	std::vector<char*> args_Vec;
	//Jump over path to exe
	for (size_t i = 1; i < m_argc; i++)
	{
		auto& it = m_um_CommandFlags.find(m_argv[i]);

		if (it != m_um_CommandFlags.end())
		{
			size_t newOffset;
			//Jump over current string(Command string)
			std::vector<char*>& buffer = CatchArguments(i + 1, newOffset);
			//Stores the Command to the vector buffer so we can execute it later
			it->second(buffer.data());
			i += newOffset;
		}
		else
		{
			std::cerr << "Couldn't find Command: [" << m_argv[i] << "]\n";
			return false;
		}
	}
	return true;
}
std::vector<char*> FSCLT::CatchArguments(size_t offset, size_t& newOffset)
{
	std::vector<char*> args;
	for (size_t i = offset; i < m_argc; i++)
	{
		if (std::strcmp(m_argv[i], "|") != 0)
		{
			args.push_back(m_argv[i]);
		}
		else
			break;
	}

	//Jump over '|'
	newOffset = args.size() + 1;
	return args;
}