#pragma once

constexpr const char* ARG_USERINP = "@USER_INPUT@";

class BaseCommand
{
public:
	BaseCommand(const std::vector<std::string>& args) : m_v_args(args) { }
	virtual ~BaseCommand() = default;

	
	virtual void PrintUsageInfo() = 0;
	virtual void Execute();

protected:
	void BindCommand(const std::vector<std::string>& pattern, std::function<void()> func);

	std::vector<std::string> m_v_args;
	std::vector<std::pair<std::vector<std::string>, std::function<void()>>> m_v_CommandDispatch;
	

private:
	virtual void PrintError();

	

};