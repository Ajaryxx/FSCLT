#pragma once
#include "Commands/Commands.hpp"

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
	std::vector<char*> CatchArguments(size_t offset, size_t& newOffset);
private:
	template<typename T>
	void CreateCommand(char** args);


private:
	int m_argc;
	char** m_argv;
	
	std::vector<BaseCommand*> m_v_Commands;

	
	std::unordered_map<std::string, std::function<void(char** args)>> m_um_CommandFlags
	{
		//Get Tool version
		{"version", std::bind(&FSCLT::CreateCommand<CVersion>, this, std::placeholders::_1)},
		//Print information
		{"print", std::bind(&FSCLT::CreateCommand<CPrint>, this, std::placeholders::_1)}
	};

	
};
template<typename T>
void FSCLT::CreateCommand(char** args)
{
	static_assert(std::is_base_of<BaseCommand, T>::value, "T must derive from BaseCommand");

	m_v_Commands.push_back(new T(args));
}