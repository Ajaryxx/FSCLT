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
constexpr const char* SEPERATOR_STR = "\n-------------------------------\n";

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
	void SetConsoleColor(Color color);
	//default color is white
	void ResetConsoleColor();


private:
	std::string GetMessageTypeString(MessageType messageType);

	OutputLog();
	~OutputLog() = default;

	inline static OutputLog* m_OutputLog = nullptr;
};
