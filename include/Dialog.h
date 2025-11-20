#pragma once
#include "OutputLog.hpp"

enum class EReturnTypeDialog : uint8_t
{
	YES,
	NO,
	UNDEFINED,
};

class Dialog
{
public:
	static Dialog& Get()
	{
		return *m_Dialog;
	}

	EReturnTypeDialog AskDialog(const std::string& message, MessageType msgType = MessageType::INFO);

	std::string ToLower(const std::string& str) const;
	std::string ToUpper(const std::string& str) const;

private:
	//returns somehing if the User typed a correct return type
	EReturnTypeDialog GetUserReturnTypeDialog() const;
	//checks if the user typed the correct retrun type
	EReturnTypeDialog CheckUserInput(const std::string& str) const;
	
	Dialog();

	inline static Dialog* m_Dialog = nullptr;
};