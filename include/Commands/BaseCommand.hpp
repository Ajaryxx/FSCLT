#pragma once

class BaseCommand
{
public:
	BaseCommand(char** args) : m_Args(args) { }
	virtual ~BaseCommand() = default;

	
	virtual void PrintUsageInfo() = 0;
	virtual void Execute() = 0;

protected:
	char** m_Args;

private:
	virtual void PrintError();


};