#pragma once

#define BIND_COMMAND(pattern, Class, function) BindCommand(pattern, std::bind(&Class::function, this, std::placeholders::_1, std::placeholders::_2))

constexpr const char* ARG_USER_INP = "@USER_INPUT@";
constexpr const char* ARG_MULTI_INP = "@MULTI_INPUT@";
constexpr const char* ARG_PARAM_FLAGS = "@PARAM_FLAG@";


enum EFLAG_PARAM : uint8_t
{
	//No flag set
	ENONE = 0x01,
	//recursuve flag
	EFLAG_RECURSIVE = 0x02,
	//local flag
	EFLAG_LOC = 0x04,
	//info flag
	EFLAG_INFO = 0x08,
	//info flag
	EFLAG_LIST = 0x10,
};

class BaseCommandModule
{
public:
	BaseCommandModule(const std::string& commandName) : m_CommandName(commandName) {}
	virtual ~BaseCommandModule() = default;

	inline std::string GetCommandFlag() const { return m_CommandName; }

	virtual void PrintUsageInfo() const = 0;
	bool Execute();

	void InitializeArguments(const std::vector<std::string>& args);

protected:
	void BindCommand(const std::vector<std::string>& pattern, std::function<bool(const std::vector<std::string>& userArg, uint8_t paramFlag)> func);

	std::vector<std::string> m_v_args;
	std::vector<std::pair<std::vector<std::string>, std::function<bool(const std::vector<std::string>& userArgs, uint8_t ParamFlags)>>> m_v_CommandDispatch;

	void ReportInvalidCommand();

private:
	std::string m_CommandName;

	const std::unordered_map<std::string, EFLAG_PARAM> m_um_Flags
	{
		//recursive
		{"-r", EFLAG_PARAM::EFLAG_RECURSIVE},
		//local
		{"-loc", EFLAG_PARAM::EFLAG_LOC},
		//info
		{"-info", EFLAG_PARAM::EFLAG_INFO},
		//list
		{"-list", EFLAG_PARAM::EFLAG_LIST},
	};
};