#pragma once

enum class Color : uint8_t
{
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	CYAN,
	MAGENTA,
};

enum class MessageType : uint8_t
{
	INFO,
	WARNING,
	EERROR
};
enum class ConvertUnit
{
	KILOBYTE,
	MEGABYTE,
	GIGABYTE,
	TERABYTE,
	//auto detect best unit to cenvert
	AUTO
};

class OutputLog
{
public:

	static OutputLog& Get()
	{
		return *m_OutputLog;
	}
	//For sending a normal message. For e.g to output Filenames
	void SendMessage(const std::string& message, uint8_t newLines = 0, Color color = Color::WHITE);
	//For sending a status message. For e.g status of a running command
	void ReportStatus(const std::string& message, MessageType messageType = MessageType::INFO, uint8_t newLines = 0);
	void PrintDirInfo(const std::vector<std::filesystem::path>& dirPaths);
	void PrintDirInfo(const std::filesystem::path& dirPath);
	void SetSpace(uint8_t newLines);
	void Seperate();
	void SetConsoleColor(Color color);
	void CountFolderAndFiles(const std::filesystem::path& path, uint32_t& fileCount, uint32_t& folderCount) const;
	uintmax_t GetFolderSize(const std::filesystem::path& folderPath);

	//Converts bytes to a specific unit
	std::string ConvertBytesToUnit(uintmax_t bytes, ConvertUnit unit);

	//Detects the best unit for dir/file size
	double CalculateAutoSizeUnit(uintmax_t bytes, std::string& unitStr);

	//default color is white
	void ResetConsoleColor();

	bool InRange(float base, float min, float max) const;
private:
	std::string GetMessageTypeString(MessageType messageType);
	//returns the modifiy time of this file
	std::string GetFileDirTime(const std::filesystem::path& patht);

/*If its a Folder it returns "Folder"
If its a File it returns the extension of this file*/
	std::string CheckElementType(const std::filesystem::path& element) const;

	//Gets the size for this file/dir and converts it to a string
	std::string GetElementSize(const std::filesystem::path& element, ConvertUnit unit = ConvertUnit::AUTO);
	void RemoveZeros(std::string& str);

	OutputLog();
	~OutputLog() = default;

	inline static OutputLog* m_OutputLog = nullptr;
};
