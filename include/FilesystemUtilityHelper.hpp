#pragma once

enum class EConvertUnit : uint8_t
{
	KILOBYTE,
	MEGABYTE,
	GIGABYTE,
	TERABYTE,
	//auto detect best unit to cenvert
	AUTO
};
enum class ECheckSizeType : uint8_t
{
	EQUAL,
	GREATER_THAN,
	LESS_THAN,
	GREATER,
	LESS,

};


class FilesystemUtilityHelper
{
public:
	static FilesystemUtilityHelper& Get()
	{
		return *m_filesystemUtility;
	}
	//returns the info as a string
	std::vector<std::string> FormatDirectoryInfo(const std::vector<std::filesystem::path>& vec);
	//returns the info as a string
	std::string FormatDirectoryInfo(const std::filesystem::path& path);

	//returns the modifiy time of this dir/file
	std::string GetModifyTime(const std::filesystem::path& path);
	
	//returns the type of this directory element
	std::string GetElementExtension(const std::filesystem::path& path);

	//returns the count of files and folders
	void CountFolderElements(const std::filesystem::path& dir, uint32_t& folderCount, uint32_t& fileCount);

	//get the size of thise Element
	std::string GetElementSize(const std::filesystem::path& dir, EConvertUnit unit = EConvertUnit::AUTO);

	//counts the folder size and returns it as bytes
	uintmax_t CountFolderSize(const std::filesystem::path& dir);

	//converts bytes in specified unit and returnst it as a string
	std::string ConvertBytesInUnit(uintmax_t bytes, EConvertUnit unit = EConvertUnit::AUTO);

	//Checks the size of this path vector. Returns true if its bigger than the size
	//size </>/ <=/>= /== vec
	bool CheckPathVectorSize(int32_t size, const std::vector<std::filesystem::path>& vecCheckSizeType, ECheckSizeType checkType, const std::string& additionalMessage = "") const;

	//Check if its a folder or a file
	std::string CheckElementType(const std::filesystem::path& path) const;

private:
	FilesystemUtilityHelper();

	double CalculateAutoSizeUnit(uintmax_t bytes, std::string& unitStr);
	bool InRange(double base, double min, double max) const;
	std::string RemoveDecimalZeros(const std::string& bytesStr);

	bool IsProtected(const std::filesystem::path& path) const;

	inline static FilesystemUtilityHelper* m_filesystemUtility = nullptr;

};