#pragma once
#include "Commands/Commands.hpp"

#define ADD_CMD(String, Command) {String, std::bind(&FSCLT::CreateCommand<Command>, this, std::placeholders::_1, std::placeholders::_2)}

class BaseCommand;

class FSCLT
{
public:
	FSCLT(int argc, const std::vector<std::string>& argv);
	~FSCLT() = default;

	//returns false when fails, true when succses
	bool Run();

	static FSCLT& Get();

	const std::vector<BaseCommand*>& GetAllCommands() const;
	
	//Can return nullptr
	BaseCommand* GetCommand(const std::string& name) const;

private:
	bool Parse();
	std::vector<std::string> CatchArguments(size_t offset, size_t& newOffset);

private:
	template<typename T>
	BaseCommand* CreateCommand(bool storeBuffer = true, const std::vector<std::string>& args = std::vector<std::string>());


private:
	int m_Argc;
	std::vector<std::string> m_Argv;

	static FSCLT* fsclt;
	
	//Commads stored to execute it later
	std::vector<BaseCommand*> m_v_Commands;
	//Commands stored which will be deleted after the last Command execution 
	std::vector<BaseCommand*> m_v_TempCommandBuffer;
	
	std::unordered_map<std::string, std::function<BaseCommand*(bool storeBuffer, const std::vector<std::string>& args)>> m_um_CommandFlags
	{
		//Print useful information like version or commands
		ADD_CMD("print", CPrint)

	};
	
};

template<typename T>
BaseCommand* FSCLT::CreateCommand(bool storeBuffer, const std::vector<std::string>& args)
{
	static_assert(std::is_base_of<BaseCommand, T>::value, "T must derive from BaseCommand");

	BaseCommand* cmd = new T(args);

	if (storeBuffer)
		m_v_Commands.push_back(cmd);
	else
		m_v_TempCommandBuffer.push_back(cmd);

	return cmd;
}