#include "PCH.hpp"
#include "FilesystemUtilityHelper.hpp"
#include "OutputLog.hpp"

namespace fs = std::filesystem;

FilesystemUtilityHelper::FilesystemUtilityHelper()
{
	this->m_filesystemUtility = this;
}

std::string FilesystemUtilityHelper::FormatDirectoryInfo(const std::vector<std::filesystem::path>& vec)
{
	std::string outStr;

	for (const auto& item : vec)
	{
		
		outStr.append("Name: " + item.filename().string()).push_back('\n');
		outStr.append("Full path: " + item.string()).push_back('\n');
		outStr.append("Date modified: " + GetModifyTime(item)).push_back('\n');
		outStr.append("Type: " + GetElementExtension(item)).push_back('\n');

		if (fs::is_directory(item))
		{
			uint32_t fileCount, folderCount;
			CountFolderElements(item, folderCount, fileCount);
			outStr.append("Folder count: " + std::to_string(folderCount)).push_back('\n');
			outStr.append("File count: " + std::to_string(fileCount)).push_back('\n');
		}

		outStr.append("Size: " + GetElementSize(item)).push_back('\n');

		outStr.append(SEPERATOR_STR);
	}

	return outStr;
}
std::string FilesystemUtilityHelper::FormatDirectoryInfo(const std::filesystem::path& path)
{
	OutputLog::Get().Seperate();

	std::string outStr;

	outStr.append("Name: " + path.filename().string()).push_back('\n');
	outStr.append("Full path: " + path.string()).push_back('\n');
	outStr.append("Date modified: " + GetModifyTime(path)).push_back('\n');
	outStr.append("Type: " + GetElementExtension(path)).push_back('\n');

	if (fs::is_directory(path))
	{
		uint32_t fileCount, folderCount;
		CountFolderElements(path, folderCount, fileCount);
		outStr.append("Folder count: " + std::to_string(folderCount)).push_back('\n');
		outStr.append("File count: " + std::to_string(fileCount)).push_back('\n');
	}
	outStr.append("Size: " + GetElementSize(path)).push_back('\n');

	return outStr;
}
//get the size of thise Element
std::string FilesystemUtilityHelper::GetElementSize(const std::filesystem::path& dir, EConvertUnit unit)
{
	if (IsProtected(dir))
	{
		OutputLog::Get().ReportStatus("[" + dir.filename().string() + "] is a protected file/folder. Can't indetify size.", MessageType::WARNING);
		return std::string("UNDEFINED");
	}
	
	std::string sizeStr;
	uintmax_t SizeBytes;
	if (fs::is_directory(dir))
	{
		
		SizeBytes = CountFolderSize(dir);
	}
	else
	{
		SizeBytes = fs::file_size(dir);
	}
	sizeStr = ConvertBytesInUnit(SizeBytes, unit);
	sizeStr = RemoveDecimalZeros(sizeStr);

	return sizeStr;
}
std::string FilesystemUtilityHelper::RemoveDecimalZeros(const std::string& bytesStr)
{
	std::string nonDecimalString = bytesStr;

	//jump over the 2 numbers before the dot
	const int StartPos = nonDecimalString.find('.') + 2;
	
	for (auto it = nonDecimalString.begin() + StartPos; it != nonDecimalString.end();)
	{
		if (*it == '0')
		{
			it = nonDecimalString.erase(it);
		}
		else
		{
			it++;
		}
	}

	return nonDecimalString;
}
std::string FilesystemUtilityHelper::ConvertBytesInUnit(uintmax_t bytes, EConvertUnit unit)
{
	double convertedUnit = 0.f;
	double sizef = static_cast<double>(bytes);
	std::string unitStr;

	switch (unit)
	{
	case EConvertUnit::KILOBYTE:
		convertedUnit = sizef / pow(1024, 1);
		unitStr = "KiB";
		break;
	case EConvertUnit::MEGABYTE:
		convertedUnit = sizef / pow(1024, 2);
		unitStr = "MiB";
		break;
	case EConvertUnit::GIGABYTE:
		convertedUnit = sizef / pow(1024, 3);
		unitStr = "GiB";
		break;
	case EConvertUnit::TERABYTE:
		convertedUnit = sizef / pow(1024, 4);
		unitStr = "TiB";
		break;
	case EConvertUnit::AUTO:
		convertedUnit = CalculateAutoSizeUnit(bytes, unitStr);
		break;
	default:
		OutputLog::Get().ReportStatus("Invalid unit specified. Returning in bytes.", MessageType::WARNING, 1);
		convertedUnit = bytes;
		break;
	}

	convertedUnit = round(convertedUnit * 100.f) / 100.f;
	std::string strConvertUnit = std::to_string(convertedUnit).append(" ").append(unitStr);
	return strConvertUnit;
}
double FilesystemUtilityHelper::CalculateAutoSizeUnit(uintmax_t bytes, std::string& unitStr)
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
uintmax_t FilesystemUtilityHelper::CountFolderSize(const std::filesystem::path& dir)
{
	uintmax_t count = 0;
	if (IsProtected(dir))
	{
		OutputLog::Get().ReportStatus("[" + dir.filename().string() + "] is a protected folder. Can't identify Folder size", MessageType::WARNING);
		return 0;
	}
	try
	{
		for (const auto& item : fs::recursive_directory_iterator(dir))
		{
			count += fs::file_size(item);
		}

	}
	catch (const fs::filesystem_error& err)
	{
		OutputLog::Get().ReportStatus("ERROR: " + std::string(err.what()), MessageType::WARNING);
	}

	return count;
}
void FilesystemUtilityHelper::CountFolderElements(const std::filesystem::path& dir, uint32_t& folderCount, uint32_t& fileCount)
{
	uint32_t DirCounter = 0; 
	uint32_t FileCounter = 0;
	if (IsProtected(dir))
	{
		folderCount = 0;
		fileCount = 0;

		OutputLog::Get().ReportStatus("[" + dir.filename().string() + "] is a protected folder. Can't count elements in this folder", MessageType::WARNING);
		return;
	}
	try
	{
		for (const auto& item : fs::recursive_directory_iterator(dir))
		{
			if (fs::is_directory(item))
			{
				DirCounter++;
			}
			else
			{
				FileCounter++;
			}
		}
	}
	catch (const fs::filesystem_error& err)
	{
		OutputLog::Get().ReportStatus("ERROR: " + std::string(err.what()), MessageType::WARNING);
	}

	folderCount = DirCounter;
	fileCount = FileCounter;
}
std::string FilesystemUtilityHelper::GetModifyTime(const std::filesystem::path& path)
{
	auto clock = std::chrono::clock_cast<std::chrono::system_clock, std::chrono::file_clock>(fs::last_write_time(path));

	const std::time_t time = std::chrono::system_clock::to_time_t(clock);

	const char* CStrTime = std::asctime(std::localtime(&time));
	if (!CStrTime)
	{
		OutputLog::Get().ReportStatus("Couldn't convert modify date to string", MessageType::EERROR);
		return std::string("[UNDEFINED]");
	}
	std::string strTime(CStrTime);

	//remove '\n'
	strTime.pop_back();

	return strTime;
}
std::string FilesystemUtilityHelper::GetElementExtension(const std::filesystem::path& path)
{
	if (fs::is_directory(path))
	{
		return std::string("Folder");
	}
	else if (fs::is_regular_file(path))
	{
		std::string type = path.extension().string();

		if (type.empty())
			return std::string("NO EXTENSION");

		return type;
	}
	else if (fs::is_symlink(path))
	{
		return std::string("Systemlink");
	}

	return std::string("[UNDEFINED]");
}
bool FilesystemUtilityHelper::InRange(double base, double min, double max) const
{
	return base >= min && base < max;
}
bool FilesystemUtilityHelper::IsProtected(const std::filesystem::path& path) const
{
#ifdef _WIN32
	
	DWORD att = GetFileAttributesW(path.c_str());
	if (att & (FILE_ATTRIBUTE_SYSTEM))
		return true;

#endif // _WIN32

	return false;
}

bool FilesystemUtilityHelper::CheckPathVectorSize(int32_t size, const std::vector<std::filesystem::path>& vec, ECheckSizeType checkType, const std::string& additionalMessage) const
{
	bool IsTrue = false;
	switch (checkType)
	{
		case ECheckSizeType::EQUAL:
			IsTrue = size == vec.size();
			break;
		case ECheckSizeType::GREATER_THAN:
			IsTrue = size <= vec.size();
			break;
		case ECheckSizeType::LESS_THAN:
			IsTrue = size >= vec.size();
			break;
		case ECheckSizeType::GREATER:
			IsTrue = size < vec.size();
			break;
		case ECheckSizeType::LESS:
			IsTrue = size > vec.size();
			break;
		default:
			OutputLog::Get().ReportStatus("Invalid check type", MessageType::EERROR);
			IsTrue = false;
			break;
	}
	if (IsTrue)
		OutputLog::Get().ReportStatus("More than " + std::to_string(size) + " elements found. " + additionalMessage, MessageType::WARNING);

	return IsTrue;
}
std::string FilesystemUtilityHelper::CheckElementType(const std::filesystem::path& path) const
{
	return fs::is_directory(path) ? "Folder" : "File";
}