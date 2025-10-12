#pragma once

class BaseCommand
{
public:
	BaseCommand(const std::vector<std::string>& args) : m_v_args(args) { }
	virtual ~BaseCommand() = default;

	
	virtual void PrintUsageInfo() = 0;
	virtual void Execute() = 0;

protected:
	std::vector<std::string> m_v_args;

private:
	virtual void PrintError();


};