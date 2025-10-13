#pragma once
#include "Commands/BaseCommand.hpp"


class CPrint : public BaseCommand
{
public:

	CPrint(const std::vector<std::string>& args);

private:

	// Inherited via BaseCommand
	void PrintUsageInfo() override;
	void Execute() override;

};
