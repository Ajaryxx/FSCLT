#pragma once
#include "Commands/BaseCommand.hpp"

constexpr const char* CSEARCH_NAME = "SEARCH";

class CSearch : public BaseCommand
{
public:
	CSearch(const std::vector<std::string>& args);

private:

	virtual void PrintUsageInfo() const;
	bool HandleSearchElement(const std::vector<std::string>& UserArgs, uint8_t flags);
};