#pragma once
#include "Commands/BaseCommand.hpp"

//Prints out the current Tool Version
class CVersion : public BaseCommand
{
public:
	CVersion() = default;
	~CVersion() = default;

private:

	// Inherited via BaseCommand
	void PrintUsageInfo() override;
	void Execute() override;
};