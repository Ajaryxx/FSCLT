#pragma once
#include "Utility/HelperMacros.hpp"


class BaseCommandModule;

class FSCLT
{
public:
	FSCLT(int argc, const std::vector<std::string>& argv);
	FSCLT(const FSCLT&) = delete;

	~FSCLT();

	FSCLT operator=(const FSCLT&) = delete;

public:
	//Common
	
	//returns false when fails, true when succses
	bool Run();

	inline static FSCLT& Get() { return *fsclt; }

	//Gets the path where fsclt is currenty executed
	inline std::filesystem::path GetExecutePath() const { return std::filesystem::current_path(); }

private:
	//Command parsing

	bool ParseCommandLine();
	//Catches all the arguments and stops until the "prefix" is reached
	std::vector<std::string> CatchArguments(size_t offset, size_t& newOffset, char prefix);

private:
	//Common

	int m_Argc;
	std::vector<std::string> m_Argv;
	static FSCLT* fsclt;

	void InitzializeCommands();
	
	//returns false if the execution of a command has failed
	bool ExecuteCommands();

private:
	//command storage

	std::vector<BaseCommandModule*> m_v_Commands;
	std::unordered_map<std::string, BaseCommandModule*> m_um_CommandFlags;
	
	template<typename T>
	void DelcareCommand(const std::string& CommandModulePrefix);

	//Pushes the command to the temp or Command buffer
	template<typename T>
	BaseCommandModule* PushCommand(const std::string& CommandModulePrefix);
	
};

template<typename T>
BaseCommandModule* FSCLT::PushCommand(const std::string& CommandModulePrefix)
{
	BaseCommandModule* cmd = new T();

	REQUIRED_ASSERT(m_um_CommandFlags.find(CommandModulePrefix) != m_um_CommandFlags.end(), "Command Prefix exists already");

	m_um_CommandFlags[CommandModulePrefix] = cmd;

	return cmd;
}

template<typename T>
void FSCLT::DelcareCommand(const std::string& CommandModulePrefix)
{
	STATIC_IS_BASE_OF(BaseCommandModule, T, "T must derive from BaseCommandModule");
	REQUIRED_ASSERT(m_um_CommandFlags.find(CommandModulePrefix) != m_um_CommandFlags.end(), "There is already a Command declared with the same Command Flag");

	PushCommand<T>(CommandModulePrefix);
}