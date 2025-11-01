#pragma once

enum class ConvertUnit
{
	KILOBYTE,
	MEGABYTE,
	GIGABYTE,
	TERABYTE,
	//auto detect best unit to cenvert
	AUTO
};

class FilesystemFormatHelper
{
public:
	static FilesystemFormatHelper& Get()
	{
		return *m_filesystemUtility;
	}
	//returns the info as a string
	std::string FormatDirectoryInfo(const std::vector<std::filesystem::path>& vec);
	//returns the info as a string
	std::string FormatDirectoryInfo(const std::filesystem::path& path);

	//returns the modifiy time of this dir/file
	std::string GetModifyTime(const std::filesystem::path& path);
	
	//returns the type of this directory element
	std::string GetElementType(const std::filesystem::path& path);

	//returns the count of files and folders
	void CountFolderElements(const std::filesystem::path& dir, uint32_t& folderCount, uint32_t& fileCount);

	//get the size of thise Element
	std::string GetElementSize(const std::filesystem::path& dir, ConvertUnit unit = ConvertUnit::AUTO);

	//counts the folder size and returns it as bytes
	uintmax_t CountFolderSize(const std::filesystem::path& dir);

	//converts bytes in specified unit and returnst it as a string
	std::string ConvertBytesInUnit(uintmax_t bytes, ConvertUnit unit = ConvertUnit::AUTO);

private:
	FilesystemFormatHelper();

	double CalculateAutoSizeUnit(uintmax_t bytes, std::string& unitStr);
	bool InRange(double base, double min, double max) const;
	std::string RemoveDecimalZeros(const std::string& bytesStr);

	bool IsProtected(const std::filesystem::path& path) const;

	inline static FilesystemFormatHelper* m_filesystemUtility = nullptr;

};