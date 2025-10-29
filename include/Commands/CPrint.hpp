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

	bool HandlePrintInfoDirectory(const std::vector<std::string>& UserArgs);
	bool HandlePrintInfoFile(const std::vector<std::string>& UserArgs);
	
	//returns all found files with a specific extension
	//NOTE: PLEASE MAKE SURE THE AT A DOT IN FORNT
	std::vector<std::filesystem::path> CheckFileExtInDir(const std::filesystem::path& dir, const std::string& ext) const;
};
