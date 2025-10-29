#include "PCH.hpp"
#include "OutputLog.hpp"

namespace fs = std::filesystem;

OutputLog::OutputLog()
{
	m_OutputLog = this;
}
void OutputLog::SendMessage(const std::string& message, uint8_t newLines, Color color)
{
	SetConsoleColor(color);

	std::cout << message << std::endl;

	ResetConsoleColor();
	SetSpace(newLines);
}
void OutputLog::ReportStatus(const std::string& message, MessageType messageType, uint8_t newLines)
{
	std::string msgStr = GetMessageTypeString(messageType);
	msgStr.append(message);

	switch (messageType)
	{
	case MessageType::INFO:
		SetConsoleColor(Color::GREEN);
		std::cout << msgStr << std::endl;
		break;
	case MessageType::WARNING:
		SetConsoleColor(Color::YELLOW);
		std::cout << msgStr << std::endl;
		break;
	case MessageType::EERROR:
		SetConsoleColor(Color::RED);
		std::cerr << msgStr << std::endl;
		break;
	default:
		SetConsoleColor(Color::MAGENTA);
		std::cerr << msgStr << std::endl;
		break;
	}

	ResetConsoleColor();
	SetSpace(newLines);
}
void OutputLog::Seperate()
{
	SetSpace(1);
	SendMessage("-------------------------------");
	SetSpace(1);
}
void OutputLog::SetSpace(uint8_t newLines)
{
	for (uint8_t i = 0; i < newLines; i++)
		std::cout << "\n";
}
std::string OutputLog::GetMessageTypeString(MessageType messageType)
{
	std::string msgStr;

	switch (messageType)
	{
	case MessageType::INFO:
		msgStr = "[INFO]: ";
		break;
	case MessageType::WARNING:
		msgStr = "[WARNING]: ";
		break;
	case MessageType::EERROR:
		msgStr = "[ERROR]: ";
		break;
	default:
		msgStr = "[UNDEFIEND STATUS]: ";
		break;
	}

	return msgStr;
}


void OutputLog::ResetConsoleColor()
{
	std::cout << "\033[0m";
}
void OutputLog::SetConsoleColor(Color color)
{
	ResetConsoleColor();
	switch (color)
	{
		case Color::WHITE:
			std::cout << "\033[37m";
			break;
		case Color::RED:
			std::cout << "\033[31m";
			break;
		case Color::GREEN:
			std::cout << "\033[32m";
			break;
		case Color::BLUE:
			std::cout << "\033[34m";
			break;
		case Color::YELLOW:
			std::cout << "\033[33m";
			break;
		case Color::CYAN:
			std::cout << "\033[36m";
			break;
		case Color::MAGENTA:
			std::cout << "\033[35m";
			break;
		default:
			std::cout << "\033[31m";
			std::cerr << "undefined color!";
			break;
	}
}