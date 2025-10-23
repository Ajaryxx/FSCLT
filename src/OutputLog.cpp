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
		SetConsoleColor(Color::WHITE);
		std::cout << msgStr << std::endl;
		break;
	case MessageType::WARNING:
		SetConsoleColor(Color::YELLOW);
		std::cout << msgStr << std::endl;
		break;
	case MessageType::ERROR:
		SetConsoleColor(Color::RED);
		std::cerr << msgStr << std::endl;
		break;
	default:
		break;
	}

	ResetConsoleColor();
	SetSpace(newLines);
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
	case MessageType::ERROR:
		msgStr = "[ERROR]: ";
		break;
	default:
		break;
	}

	return msgStr;
}

void OutputLog::PrintDirFileInfoVec(const std::vector<std::filesystem::path> buffer)
{
	size_t counter = 0;
	for (const auto& item : buffer)
	{
		SendMessage("Name: " + item.filename().string());

		const std::string TimeStr = GetFileDirTime(item);
		SendMessage("Date modified: " + TimeStr);

		std::string ext = item.extension().string();
		if(ext.empty())
			SendMessage("Type: Folder");
		else
			SendMessage("Type: " + ext);
		
		uintmax_t fileSize = GetFolderSize(item);
		double a = ConvertFileDirSize(fileSize, ConvertUnit::MEGABYTE);
		SendMessage("Size: " + std::to_string(a));

		counter++;
	}
	
}
std::string OutputLog::GetFileDirTime(const fs::path& path)
{
	auto timeCast = std::chrono::clock_cast<std::chrono::system_clock, std::chrono::file_clock>(fs::last_write_time(path));

	std::time_t time = std::chrono::system_clock::to_time_t(timeCast);

	char* cTime = std::asctime(std::localtime(&time));
	if (!cTime)
	{
		ReportStatus("Couldn't convert time to string", MessageType::ERROR);
		return std::string("[UNDEFINED]");
	}
	std::string timeStr(cTime);
	timeStr.pop_back();
	return timeStr;
}
uintmax_t OutputLog::GetFolderSize(const std::filesystem::path& folderPath) const
{
	uintmax_t size = 0;
	for (const auto& item : fs::recursive_directory_iterator(folderPath))
	{
		size += item.file_size();
	}
	return size;
}

uintmax_t OutputLog::ConvertFileDirSize(uintmax_t size, ConvertUnit unit)
{
	uintmax_t convertedUnit = 0.f;
	switch (unit)
	{
	case ConvertUnit::KILOBYTE:
		convertedUnit = size / 1024;
		break;
	case ConvertUnit::MEGABYTE:
		convertedUnit = size / 1048576;
		break;
	case ConvertUnit::GIGABYTE:
		break;
	case ConvertUnit::TERABYTE:
		break;
	default:
		break;
	}
	return convertedUnit;
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