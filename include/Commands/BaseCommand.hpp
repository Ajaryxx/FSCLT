#pragma once

class BaseCommand
{
public:
	BaseCommand() = default;
	virtual ~BaseCommand() = default;

	
	virtual void PrintUsageInfo() = 0;
	virtual void Execute() = 0;
private:
	virtual void PrintError();

};