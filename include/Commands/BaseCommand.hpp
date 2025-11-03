#pragma once

#define BIND_COMMAND(pattern, function) BindCommand(pattern, std::bind(&CPrint::function, this, std::placeholders::_1, std::placeholders::_2))

constexpr const char* ARG_USER_INP = "@USER_INPUT@";
constexpr const char* ARG_MULTI_INP = "@MULTI_INPUT@";
constexpr const char* ARG_PARAM_FLAGS = "@PARAM_FLAG@";


enum EFLAG_PARAM : uint8_t
{
	//No flag set
	ENONE = 0x00,
	//List flag
	EFLAG_LIST = 0x01,
	//recursuve flag
	EFLAG_RECURSIVE = 0x02,
	//local flag
	EFLAG_LOC = 0x04,
	//info flag
	EFLAG_INFO = 0x08,
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
	void BindCommand(const std::vector<std::string>& pattern, std::function<bool(const std::vector<std::string>& userArg, uint8_t paramFlag)> func);

	std::vector<std::string> m_v_args;
	std::vector<std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs, uint8_t ParamFlags)>>> m_v_CommandDispatch;
	void ReportInvalidCommand();

	//Extract all flags
	std::vector<std::string> ExtractParamFlags(const std::vector<std::string>& userParams);

	uint8_t GetParamFlagsAsFlag(const std::vector<std::string>& flagsVec) const;

private:
	std::string m_CommandName;

	//Parses the command. If it returns false the command couldn't found
	bool ParseCommand(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs, uint8_t& paramFlag);

	const std::unordered_map<std::string, EFLAG_PARAM> m_um_Flags
	{
		//recursive
		{"-r", EFLAG_PARAM::EFLAG_RECURSIVE},
		//list
		{"-l", EFLAG_PARAM::EFLAG_LIST},
		//local
		{"-loc", EFLAG_PARAM::EFLAG_LOC},
		//info
		{"-i", EFLAG_PARAM::EFLAG_INFO},
	};
};