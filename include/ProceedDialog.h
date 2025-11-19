#pragma once
#include "OutputLog.hpp"

enum class EReturnTypeDialog : uint8_t
{
	OK,
	CANCEL,
	YES,
	NO,

};

class ProceedDialog
{
public:
	static ProceedDialog& Get()
	{
		return *m_ProceedDialog;
	}

	EReturnTypeDialog AskDialog(const std::string& message, MessageType msgType = MessageType::INFO);

	std::string ToLower(const std::string& str) const;
	std::string ToUpper(const std::string& str) const;

private:
	ProceedDialog();

	inline static ProceedDialog* m_ProceedDialog = nullptr;
};