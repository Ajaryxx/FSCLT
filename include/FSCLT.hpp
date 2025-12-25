#pragma once

#define DECLARE_COMMAND_FLAG(CommandFlag, Class) DelcareCommand<Class>(CommandFlag)

class BaseCommandModule;

class FSCLT
{
public:
	FSCLT(int argc, const std::vector<std::string>& argv);
	~FSCLT();

	//returns false when fails, true when succses
	bool Run();

	static FSCLT& Get();
	

	const std::vector<BaseCommandModule*>& GetAllCommands() const;
	
	//Can return nullptr
	BaseCommandModule* GetCommand(const std::string& name) const;

	//Gets the path where fsclt is currenty executed
	std::filesystem::path GetExecutePath() const
	{
		//we want always know the current directory
		return std::filesystem::current_path();
	}
private:
	
	bool ParseCommandLine();
	std::vector<std::string> CatchArguments(size_t offset, size_t& newOffset);

private:
	//Pushes the command to the temp or Command buffer
	template<typename T>
	BaseCommandModule* PushCommand(bool storeBuffer = true, const std::vector<std::string>& args = std::vector<std::string>());

private:
	int m_Argc;
	std::vector<std::string> m_Argv;
	
	static FSCLT* fsclt;

	//Commads stored to execute it later
	std::vector<BaseCommandModule*> m_v_Commands;
	//Commands stored which will be deleted after the last Command execution 
	std::vector<BaseCommandModule*> m_v_TempCommandBuffer;
	
	std::unordered_map<std::string, std::function<BaseCommandModule* (bool storeBuffer, const std::vector<std::string>& args)>> m_um_CommandFlags;

	void InitzializeCommands();

	template<typename T>
	void DelcareCommand(const std::string& cmdFlag);
};

template<typename T>
BaseCommandModule* FSCLT::PushCommand(bool storeBuffer, const std::vector<std::string>& args)
{
	static_assert(std::is_base_of<BaseCommandModule, T>::value, "T must derive from BaseCommandModule");

	BaseCommandModule* cmd = new T(args);

	if (storeBuffer)
		m_v_Commands.push_back(cmd);
	else
		m_v_TempCommandBuffer.push_back(cmd);

	return cmd;
}

template<typename T>
void FSCLT::DelcareCommand(const std::string& cmdFlag)
{
	static_assert(std::is_base_of<BaseCommandModule, T>::value, "T must derive from BaseCommandModule");
	assert(!(m_um_CommandFlags.find(cmdFlag) != m_um_CommandFlags.end()) && "There is already a Command declared with the same Command Flag");

	m_um_CommandFlags[cmdFlag] = std::bind(&FSCLT::PushCommand<T>, this, std::placeholders::_1, std::placeholders::_2);
}