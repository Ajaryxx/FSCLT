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

class OutputLog
{
public:

	static OutputLog& Get()
	{
		return *m_OutputLog;
	}
	//For sending a normal message. For e.g to output Filenames
	void SendOutput(const std::string& message, uint8_t newLines = 0, Color color = Color::WHITE);
	//For sending a status message. For e.g status of a running command
	void ReportStatus(const std::string& message, MessageType messageType = MessageType::INFO, uint8_t newLines = 1);

	void SetSpace(uint8_t newLines);
	void Seperate();

	void PrintList(const std::vector<std::string>& list, const std::string& listName, Color color = Color::WHITE);

private:
	std::string GetMessageTypeString(MessageType messageType);

	OutputLog();
	~OutputLog() = default;

	std::string MakeSeperatorWithLenght(size_t size);
	void SetConsoleColor(Color color);
	//default color is white
	void ResetConsoleColor();

	inline static OutputLog* m_OutputLog = nullptr;
};
