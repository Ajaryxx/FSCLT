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
		std::cerr << "Usage: fsclt [Command Flag] [ARGS] | [Command Flag] [ARGS]...\n";
		return false;
	}
	Parse();
	
}
bool FSCLT::Parse()
{
	//Ignore path to exe
	for (size_t i = 1; i < m_argc; i++)
	{
		auto& it = m_um_CommandFlags.find(m_argv[i]);

		if (it != m_um_CommandFlags.end())
		{
			it->second();
		}
		else
		{
			std::cerr << "Could not find Command: " << m_argv[i] << std::endl;
			return false;
		}
	}
	return true;
}
void FSCLT::CMD_VERSION()
{
	m_v_Commands.push_back(new CVersion());
}