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

void OutputLog::PrintDirInfo(const std::vector<std::filesystem::path>& dirPaths)
{
	for (const auto& item : dirPaths)
	{
		SendMessage("Name: " + item.filename().string());

		const std::string TimeStr = GetFileDirTime(item);
		SendMessage("Date modified: " + TimeStr);

		SendMessage("Type: " + CheckElementType(item));
		
		uintmax_t FolderSize = GetFolderSize(item);
		std::string ConvFolderSize = ConvertBytesToUnit(FolderSize, ConvertUnit::AUTO);
		SendMessage("Size: " + ConvFolderSize);

		
		uint32_t FolderCount;
		uint32_t FileCount;
		CountFolderAndFiles(item, FileCount, FolderCount);
		SendMessage("Folder Count: " + std::to_string(FolderCount).append("\n") +
				"File Count: " + std::to_string(FileCount));

		Seperate();
	}
	
}
void OutputLog::PrintDirInfo(const std::filesystem::path& dirPath)
{
	SendMessage("Name: " + dirPath.filename().string());

	const std::string TimeStr = GetFileDirTime(dirPath);
	SendMessage("Date modified: " + TimeStr);

	SendMessage("Type: " + CheckElementType(dirPath));

	uintmax_t FolderSize = GetFolderSize(dirPath);
	std::string ConvFolderSize = ConvertBytesToUnit(FolderSize, ConvertUnit::AUTO);
	SendMessage("Size: " + ConvFolderSize);

	uint32_t FolderCount;
	uint32_t FileCount;
	CountFolderAndFiles(dirPath, FileCount, FolderCount);
	SendMessage("Folder Count: " + std::to_string(FolderCount).append("\n") +
	"File Count: " + std::to_string(FileCount));

	Seperate();
}
void OutputLog::PrintFileInfo(const std::vector<std::filesystem::path>& filePaths)
{
	for (const auto& item : filePaths)
	{
		SendMessage("Name: " + item.filename().string());

		const std::string TimeStr = GetFileDirTime(item);
		SendMessage("Date modified: " + TimeStr);

		SendMessage("Type: " + CheckElementType(item));

		SendMessage("Size: " + GetElementSize(item));


		Seperate();
	}

	
}
void OutputLog::PrintFileInfo(const std::filesystem::path& filePath)
{

	SendMessage("Name: " + filePath.filename().string());

	const std::string TimeStr = GetFileDirTime(filePath);
	SendMessage("Date modified: " + TimeStr);

	SendMessage("Type: " + CheckElementType(filePath));

	SendMessage("Size: " + GetElementSize(filePath));

	Seperate();
}
std::string OutputLog::GetFileDirTime(const fs::path& path)
{
	auto timeCast = std::chrono::clock_cast<std::chrono::system_clock, std::chrono::file_clock>(fs::last_write_time(path));

	const std::time_t time = std::chrono::system_clock::to_time_t(timeCast);

	char* cTime = std::asctime(std::localtime(&time));
	if (!cTime)
	{
		ReportStatus("Couldn't convert time to string", MessageType::EERROR);
		return std::string("[UNDEFINED]");
	}

	std::string timeStr(cTime);
	//remove \n at the end
	timeStr.pop_back();

	return timeStr;
}
uintmax_t OutputLog::GetFolderSize(const std::filesystem::path& folderPath)
{
	uintmax_t size = 0;
	try
	{
		for (const auto& item : fs::recursive_directory_iterator(folderPath))
		{
			size += item.file_size();
		}
		return size;
	}
	catch (const fs::filesystem_error& err)
	{
		ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR, 1);
	}
	
	return 0;
}
void OutputLog::CountFolderAndFiles(const fs::path& path, uint32_t& fileCount, uint32_t& folderCount)
{
	
	uint32_t FolderCount = 0;
	uint32_t FilesCount = 0;
	try
	{
		for (const auto& item : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied))
		{
			if (item.is_directory())
				FolderCount++;
			else
				FilesCount++;
		}

	}
	catch (const fs::filesystem_error& err)
	{
		ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR, 1);
	}
	
	fileCount = FilesCount;;
	folderCount = FolderCount;
}
std::string OutputLog::ConvertBytesToUnit(uintmax_t bytes, ConvertUnit unit)
{	
	double convertedUnit = 0.f;
	double sizef = static_cast<double>(bytes);
	std::string unitStr;

	switch (unit)
	{
	case ConvertUnit::KILOBYTE:
		convertedUnit = sizef / pow(1024, 1);
		unitStr = "KiB";
		break;
	case ConvertUnit::MEGABYTE:
		convertedUnit = sizef / pow(1024, 2);
		unitStr = "MiB";
		break;
	case ConvertUnit::GIGABYTE:
		convertedUnit = sizef / pow(1024, 3);
		unitStr = "GiB";
		break;
	case ConvertUnit::TERABYTE:
		convertedUnit = sizef / pow(1024, 4);
		unitStr = "TiB";
		break;
	case ConvertUnit::AUTO:
		convertedUnit = CalculateAutoSizeUnit(bytes, unitStr);
		break;
	default:
		ReportStatus("Invalid unit specified. Returning in bytes.", MessageType::WARNING, 1);
		convertedUnit = bytes;
		break;
	}
	
	convertedUnit = round(convertedUnit * 100.f) / 100.f;
	std::string strConvertUnit = std::to_string(convertedUnit).append(" ").append(unitStr);
	RemoveZeros(strConvertUnit);
	return strConvertUnit;
}
double OutputLog::CalculateAutoSizeUnit(uintmax_t bytes, std::string& unitStr)
{
	double convertedUnit = 0;
	double sizef = static_cast<float>(bytes);
	
	if (InRange(bytes, 0, pow(1024, 1)))
	{
		//byte
		convertedUnit = sizef;
		unitStr = "BYTES";
	}
	else if (InRange(bytes, pow(1024, 1), pow(1024, 2)))
	{
		//kilobyte
		convertedUnit = sizef / pow(1024, 1);
		unitStr = "KiB";
	}
	else if (InRange(bytes, pow(1024, 2), pow(1024, 3)))
	{
		//megabyte
		convertedUnit = sizef / pow(1024, 2);
		unitStr = "MiB";
	}
	else if (InRange(bytes, pow(1024, 3), pow(1024, 4)))
	{
		//gigabyte
		convertedUnit = sizef / pow(1024, 3);
		unitStr = "GiB";
	}
	else
	{
		//terabyte
		convertedUnit = sizef / pow(1024, 4);
		unitStr = "TiB";
	}
	return convertedUnit;
}
void OutputLog::RemoveZeros(std::string& str)
{
	size_t pos = str.find('.');
	//jump over num before the dot
	pos += 2;

	for (auto it = str.begin() + pos; it != str.end();)
	{
		if (*it == '0')
		{
			it = str.erase(it);
		}
		else
		{
			it++;
		}
	}
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
bool OutputLog::InRange(float base, float min, float max) const
{
	return base >= min && base < max;
}
std::string OutputLog::CheckElementType(const fs::path& element) const
{
	if (fs::is_directory(element))
	{
		return "Folder";
	}
	else
	{
		return element.extension().string();
	}
}
std::string OutputLog::GetElementSize(const std::filesystem::path& element, ConvertUnit unit)
{
	const uintmax_t fileSize = fs::file_size(element);
	std::string convertedUnit = ConvertBytesToUnit(fileSize, unit);
	RemoveZeros(convertedUnit);

	return convertedUnit;
}