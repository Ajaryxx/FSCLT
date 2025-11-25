#pragma once
#include "Commands/BaseCommand.hpp"

constexpr const char* CMD_NAME = "print";

constexpr const char* HANDLE_SEARCH = "HANDLE_SEARCH";

class CPrint : public BaseCommand
{
public:

	CPrint(const std::vector<std::string>& args);

private:

	// Inherited via BaseCommand
	void PrintUsageInfo() const override;

private:
	
	bool HandlePrintOutVersion(const std::vector<std::string>& UserArgs, uint8_t flags);
	bool HandlePrintOutCommandInfo(const std::vector<std::string>& UserArgs, uint8_t flags);
	bool HandlePrintListDirectory(const std::vector<std::string>& UserArgs, uint8_t flags);
	bool HandlePrintInfoElement(const std::vector<std::string>& UserArgs, uint8_t flags);

};
