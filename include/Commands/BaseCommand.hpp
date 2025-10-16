#pragma once

#define BIND_COMMAND(pattern, function) BindCommand(pattern, std::bind(&CPrint::function, this, std::placeholders::_1))

constexpr const char* ARG_USERINP = "@USER_INPUT@";

class BaseCommand
{
public:
	BaseCommand(const char* commandName, const std::vector<std::string>& args) : m_v_args(args), m_CommandName(commandName) { }
	virtual ~BaseCommand() = default;

	
	virtual void PrintUsageInfo() = 0;
	virtual void Execute();

protected:
	void BindCommand(const std::vector<std::string>& pattern, std::function<void(const std::vector<std::string>& userArg)> func);

	std::vector<std::string> m_v_args;
	std::vector<std::pair<std::vector<std::string>, std::function<void(const std::vector<std::string>& userArgs)>>> m_v_CommandDispatch;
	

private:
	virtual void PrintError();
	const char* m_CommandName;
	

};