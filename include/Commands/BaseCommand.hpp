#pragma once

#define BIND_COMMAND(pattern, function) BindCommand(pattern, std::bind(&CPrint::function, this, std::placeholders::_1))

constexpr const char* ARG_USERINP = "@USER_INPUT@";
constexpr const char* ARG_MULTIINP = "@MULTI_INPUT@";


enum EFLAG_PARAM : uint8_t
{
	ENONE = 0x00,
	EFLAG_LIST = 0x01,
	EFLAG_RECURSIVE = 0x02,
	EFLAG_INFO = 0x04,
};

class BaseCommand
{
public:
	BaseCommand(const std::string& commandName, const std::vector<std::string>& args) : m_v_args(args), m_CommandName(commandName) {}
	virtual ~BaseCommand() = default;

	inline std::string GetCommandFlag() const
	{
			return m_CommandName;
	}

	virtual void PrintUsageInfo() = 0;
	virtual bool Execute();

protected:
	void BindCommand(const std::vector<std::string>& pattern, std::function<bool(const std::vector<std::string>& userArg)> func);

	std::vector<std::string> m_v_args;
	std::vector<std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs)>>> m_v_CommandDispatch;
	void ReportInvalidCommand();

	//Extract all flags
	std::vector<std::string> ExtractParamFlags(const std::vector<std::string>& userParams);

	uint8_t GetParamFlagsAsFlag(const std::vector<std::string>& flagsVec) const;

private:
	std::string m_CommandName;

	bool CheckEqualCommand(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs);

	const std::unordered_map<std::string, EFLAG_PARAM> m_um_Flags
	{
		//RECURSIVE
		{"-r", EFLAG_PARAM::EFLAG_RECURSIVE},
		//List
		{"-l", EFLAG_PARAM::EFLAG_LIST},
		//Info
		{"-i", EFLAG_PARAM::EFLAG_INFO},
	};
};