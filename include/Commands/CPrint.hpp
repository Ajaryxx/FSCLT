#pragma once
#include "Commands/BaseCommand.hpp"

constexpr const char* ARG_INFO = "info";
constexpr const char* ARG_COMMAND = "command";
constexpr const char* ARG_VERSION = "version";

class CPrint : public BaseCommand
{
public:

	CPrint(const std::vector<std::string>& args);

private:

	// Inherited via BaseCommand
	void PrintUsageInfo() override;
	void Execute() override;

	void HandlePrintVersion();
};
