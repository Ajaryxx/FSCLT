#pragma once
#include "Commands/BaseCommand.hpp"


class CPrint : public BaseCommand
{
public:

	CPrint(char** args) : BaseCommand(args) {}

private:

	// Inherited via BaseCommand
	void PrintUsageInfo() override;
	void Execute() override;
};
