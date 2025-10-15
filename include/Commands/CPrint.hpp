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

	void HandlePrintVersion(const std::vector<std::string>& UserArgs);
	void HandlePrintVersion2(const std::vector<std::string>& UserArgs);
};
