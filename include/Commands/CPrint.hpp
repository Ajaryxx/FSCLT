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
	bool HandlePrintInfoCommands(const std::vector<std::string>& UserArgs);
	bool HandlePrintCommandList(const std::vector<std::string>& UserArgs);

	bool HandlePrintListDirectory(const std::vector<std::string>& UserArgs);

	bool HandleSearch(const std::vector<std::string>& UserArgs);

	bool HandlePrintInfoElement(const std::vector<std::string>& UserArgs);
};
