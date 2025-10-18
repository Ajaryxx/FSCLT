#pragma once

#define BIND_COMMAND(pattern, function) BindCommand(pattern, std::bind(&CPrint::function, this, std::placeholders::_1))

constexpr const char* ARG_USERINP = "@USER_INPUT@";
constexpr const char* ARG_MULTIINP = "@MULTI_INPUT@";

class BaseCommand
{
public:
	BaseCommand(const std::string& commandName, const std::vector<std::string>& args) : m_v_args(args), m_CommandName(commandName) { }
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


private:
	std::string m_CommandName;
	
	bool CheckEqualCommand(const std::vector<std::string>& pattern, std::vector<std::string>& userArgs);

};