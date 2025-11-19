#include "PCH.hpp"
#include "ProceedDialog.h"

ProceedDialog::ProceedDialog()
{
	m_ProceedDialog = this;
}
EReturnTypeDialog ProceedDialog::AskDialog(const std::string& message, MessageType msgType)
{
	OutputLog& log = OutputLog::Get();
	const char* AskString = "Type [Y] for yes or [N] for no.";
	log.ReportStatus(message + " " + AskString, msgType);
	std::string buffer(4, '\0');
	std::cin.get(&buffer[0], 4);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	EReturnTypeDialog returnType = EReturnTypeDialog::NO;

	return returnType;
}
std::string ProceedDialog::ToLower(const std::string& str) const
{
	std::string buffer;
	buffer.reserve(str.size());
	for (auto& item : str)
	{
		buffer += tolower(item);
	}

	return buffer;
}
std::string ProceedDialog::ToUpper(const std::string& str) const
{
	std::string buffer;
	buffer.reserve(str.size());

	for (auto& item : str)
	{
		buffer += toupper(item);
	}
	return buffer;
}