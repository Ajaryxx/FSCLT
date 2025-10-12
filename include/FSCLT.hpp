#pragma once
#include "Commands/Commands.hpp"

#define ADD_CMD(String, Command) {String, std::bind(&FSCLT::CreateCommand<Command>, this, std::placeholders::_1)}

class BaseCommand;

class FSCLT
{
public:
	FSCLT(int argc, const std::vector<std::string>& argv);
	~FSCLT() = default;

	//returns false when fails, true when succses
	bool Run();

	static FSCLT& Get();

	inline const std::vector<BaseCommand*>& GetAllCommands() const
	{
		for (auto it = m_um_CommandFlags.begin(); it != m_um_CommandFlags.end(); it++)
		{
			it->second(std::vector<std::string>());
		}

		return m_v_Commands;
	}


private:
	bool Parse();
	std::vector<std::string> CatchArguments(size_t offset, size_t& newOffset);

private:
	template<typename T>
	void CreateCommand(const std::vector<std::string>& args);


private:
	int m_Argc;
	std::vector<std::string> m_Argv;

	static FSCLT* fsclt;
	
	std::vector<BaseCommand*> m_v_Commands;
	
	std::unordered_map<std::string, std::function<void(const std::vector<std::string>& args)>> m_um_CommandFlags
	{
		//Print useful information like version or commands
		ADD_CMD("print", CPrint)

	};
	
};

template<typename T>
void FSCLT::CreateCommand(const std::vector<std::string>& args)
{
	static_assert(std::is_base_of<BaseCommand, T>::value, "T must derive from BaseCommand");

	m_v_Commands.push_back(new T(args));
}