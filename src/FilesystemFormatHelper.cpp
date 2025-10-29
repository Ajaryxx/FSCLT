#include "PCH.hpp"
#include "FilesystemFormatHelper.hpp"
#include "OutputLog.hpp"

namespace fs = std::filesystem;

FilesystemFormatHelper::FilesystemFormatHelper()
{
	this->m_filesystemUtility = this;
}

std::string FilesystemFormatHelper::FormatDirectoryInfo(const std::vector<std::filesystem::path>& vec)
{
	std::string outStr;

	for (const auto& item : vec)
	{
		outStr.append("Name: " + item.filename().string()).push_back('\n');
		outStr.append("Date modifieds: " + GetModifyTime(item)).push_back('\n');
		outStr.append("Type: " + GetElementType(item)).push_back('\n');

		if (fs::is_directory(item))
		{
			uint32_t fileCount, folderCount;
			CountFolderElements(item, folderCount, fileCount);
			outStr.append("Folder count: " + std::to_string(folderCount)).push_back('\n');
			outStr.append("File count: " + std::to_string(fileCount)).push_back('\n');
		}

		outStr.append("Size: " + GetElementSize(item)).push_back('\n');

		OutputLog::Get().Seperate();
	}

	return outStr;
}
std::string FilesystemFormatHelper::FormatDirectoryInfo(const std::filesystem::path& path)
{
	std::string outStr;

	outStr.append("Name: " + path.filename().string()).push_back('\n');
	outStr.append("Date modifieds: " + GetModifyTime(path)).push_back('\n');
	outStr.append("Type: " + GetElementType(path)).push_back('\n');

	if (fs::is_directory(path))
	{
		uint32_t fileCount, folderCount;
		CountFolderElements(path, folderCount, fileCount);
		outStr.append("Folder count: " + std::to_string(folderCount)).push_back('\n');
		outStr.append("File count: " + std::to_string(fileCount)).push_back('\n');
	}
	outStr.append("Size: " + GetElementSize(path)).push_back('\n');

	OutputLog::Get().Seperate();

	return outStr;
}
//get the size of thise Element
std::string FilesystemFormatHelper::GetElementSize(const std::filesystem::path& dir, ConvertUnit unit)
{
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
std::string FilesystemFormatHelper::RemoveDecimalZeros(const std::string& bytesStr)
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
std::string FilesystemFormatHelper::ConvertBytesInUnit(uintmax_t bytes, ConvertUnit unit)
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
		OutputLog::Get().ReportStatus("Invalid unit specified. Returning in bytes.", MessageType::WARNING, 1);
		convertedUnit = bytes;
		break;
	}

	convertedUnit = round(convertedUnit * 100.f) / 100.f;
	std::string strConvertUnit = std::to_string(convertedUnit).append(" ").append(unitStr);
	return strConvertUnit;
}
double FilesystemFormatHelper::CalculateAutoSizeUnit(uintmax_t bytes, std::string& unitStr)
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
uintmax_t FilesystemFormatHelper::CountFolderSize(const std::filesystem::path& dir)
{
	uintmax_t count = 0;
	try
	{
		for (const auto& item : fs::recursive_directory_iterator(dir))
		{
			count += fs::file_size(item);
		}

	}
	catch (const fs::filesystem_error& err)
	{
		OutputLog::Get().ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR);
	}

	return count;

}
void FilesystemFormatHelper::CountFolderElements(const std::filesystem::path& dir, uint32_t& folderCount, uint32_t& fileCount)
{
	uint32_t DirCounter = 0; 
	uint32_t FileCounter = 0;
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
		OutputLog::Get().ReportStatus("ERROR: " + std::string(err.what()), MessageType::EERROR);
	}

	folderCount = DirCounter;
	fileCount = FileCounter;
}
std::string FilesystemFormatHelper::GetModifyTime(const std::filesystem::path& path)
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
std::string FilesystemFormatHelper::GetElementType(const std::filesystem::path& path)
{
	if (fs::is_directory(path))
	{
		return std::string("Folder");
	}
	else if (fs::is_regular_file(path))
	{
		return path.extension().string();
	}
	else if (fs::is_symlink(path))
	{
		return std::string("Systemlink");
	}

	return std::string("[UNDEFINED]");
}
bool FilesystemFormatHelper::InRange(double base, double min, double max) const
{
	return base >= min && base < max;
}