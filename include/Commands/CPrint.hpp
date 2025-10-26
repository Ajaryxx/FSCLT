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

	bool HandlePrintListDirectorys(const std::vector<std::string>& UserArgs);
	bool HandlePrintListFiles(const std::vector<std::string>& UserArgs);

	bool HandlePrintInfoDirectory(const std::vector<std::string>& UserArgs);
	bool HandlePrintInfoFile(const std::vector<std::string>& UserArgs);
	
	//Returns the path if the path was found or it returns an empty string if not 
	std::filesystem::path DoesExists(const std::string dirName, const std::filesystem::path& fullPath) const;
	//returns all found files with a specific extension
	//NOTE: PLEASE MAKE SURE THE AT A DOT IN FORNT
	std::vector<std::filesystem::path> CheckFileExtInDir(const std::filesystem::path& dir, const std::string& ext) const;
};
