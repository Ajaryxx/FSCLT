#pragma once
#include "CommandModules/BaseCommandModule.hpp"

constexpr const char* CSEARCH_NAME = "SEARCH";

class CSearch : public BaseCommandModule
{
public:
	CSearch(const std::vector<std::string>& args);

private:

	virtual void PrintUsageInfo() const;
	bool HandleSearchElement(const std::vector<std::string>& UserArgs, uint8_t flags);
	bool HandleSearchWordInFile(const std::vector<std::string>& UserArgs, uint8_t flags);
};