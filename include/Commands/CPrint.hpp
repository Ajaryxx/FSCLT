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
	void PrintUsageInfo() override;

	bool HandlePrintVersion(const std::vector<std::string>& UserArgs, uint8_t ParamFlag);
	bool HandlePrintInfoCommands(const std::vector<std::string>& UserArgs, uint8_t ParamFlag);
	bool HandlePrintCommandList(const std::vector<std::string>& UserArgs, uint8_t ParamFlag);

	bool HandlePrintListDirectory(const std::vector<std::string>& UserArgs, uint8_t ParamFlag);

	bool HandleSearch(const std::vector<std::string>& UserArgs, uint8_t ParamFlag);

	bool HandlePrintInfoElement(const std::vector<std::string>& UserArgs, uint8_t ParamFlag);

	bool PrintCommandsWithName(const std::vector<std::string>& UserArgs);

	bool CheckSearchRecursive(const std::string& arg);

	//Prints out all elements with a specific name
	bool PrintAllEqualNamesInArgumentList(const std::vector<std::string>& args, const std::vector<std::filesystem::path>& paths);

	std::vector<std::filesystem::path> DoRecursiveDirIterate(const std::filesystem::path& searchPath) const;
	std::vector<std::filesystem::path> DoDirIterate(const std::filesystem::path& searchPath) const;

private:

	

};
