#pragma once
#include "CommandFlagStrings.hpp"

class BaseCommand;

class FSCLT
{
public:
	FSCLT(int argc, char* argv[]);
	~FSCLT() = default;

	//returns false when fails, true when succses
	bool Run();
private:
	bool Parse();

private:
	void CMD_VERSION();

private:
	int m_argc;
	char** m_argv;
	
	std::vector<BaseCommand*> m_v_Commands;


	std::unordered_map<std::string, std::function<void()>> m_um_CommandFlags
	{
		//Get Tool version
		{"version", std::bind(&FSCLT::CMD_VERSION, this)},
	};

	
};