#pragma once
#include "Commands/BaseCommand.hpp"

constexpr const char* CMD_NAME = "print";

class CPrint : public BaseCommand
{
public:

	CPrint(const std::vector<std::string>& args);

private:

	// Inherited via BaseCommand
	void PrintUsageInfo() override;

	bool HandlePrintVersion(const std::vector<std::string>& UserArgs);
	bool HandlePrintCommands(const std::vector<std::string>& UserArgs);
	bool HandlePrintDirectorys(const std::vector<std::string>& UserArgs);
	bool HandlePrintFiles(const std::vector<std::string>& UserArgs);
	
};
