#include "PCH.hpp"
#include "Dialog.h"

Dialog::Dialog()
{
	m_Dialog = this;
}
EReturnTypeDialog Dialog::AskDialog(const std::string& message, MessageType msgType)
{
	OutputLog& log = OutputLog::Get();;

	log.ReportStatus(message, MessageType::INFO);

	EReturnTypeDialog retDia = GetUserReturnTypeDialog();

	return retDia;
}
EReturnTypeDialog Dialog::GetUserReturnTypeDialog() const
{
	bool UserParseSuccsess = true;
	EReturnTypeDialog diaReturn = EReturnTypeDialog::UNDEFINED;
	OutputLog& log = OutputLog::Get();

	do
	{
		UserParseSuccsess = true;
		log.ReportStatus("Type [Y] for yes or [N] for no.", MessageType::INFO);
	
		std::string buffer;
		std::getline(std::cin, buffer);

		diaReturn = CheckUserInput(buffer);
		if (diaReturn == EReturnTypeDialog::UNDEFINED)
		{
			OutputLog::Get().ReportStatus("Invalid input. Please try again.", MessageType::WARNING);
			UserParseSuccsess = false;
		}


	} while (!UserParseSuccsess);

	return diaReturn;
}
EReturnTypeDialog Dialog::CheckUserInput(const std::string& str) const
{
	EReturnTypeDialog diaReturn = EReturnTypeDialog::UNDEFINED;
	std::string lowerStr = ToLower(str);

	if (lowerStr.size() > 1)
	{
		if (lowerStr == "yes") diaReturn = EReturnTypeDialog::YES;
		else if (lowerStr == "no") diaReturn = EReturnTypeDialog::NO;
	}
	else
	{
		if(lowerStr[0] == 'y') diaReturn = EReturnTypeDialog::YES;
		else if (lowerStr[0] == 'n') diaReturn = EReturnTypeDialog::NO;
	}
	
	return diaReturn;
}

std::string Dialog::ToLower(const std::string& str) const
{
	std::string buffer;
	buffer.reserve(str.size());
	for (const auto& item : str)
	{
		buffer += tolower(item);
	}

	return buffer;
}
std::string Dialog::ToUpper(const std::string& str) const
{
	std::string buffer;
	buffer.reserve(str.size());

	for (const auto& item : str)
	{
		buffer += toupper(item);
	}
	return buffer;
}