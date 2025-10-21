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
	ERROR
};

class OutputLog
{
public:

	static OutputLog& Get()
	{
		return *m_OutputLog;
	}

	void SendMessage(const std::string& message, uint8_t newLines = 0, Color color = Color::WHITE);
	void ReportStatus(const std::string& message, MessageType messageType = MessageType::INFO, uint8_t newLines = 0);
	void SetSpace(uint8_t newLines);
	void SetConsoleColor(Color color);

	//default color is white
	void ResetConsoleColor();
private:
	std::string GetMessageTypeString(MessageType messageType);
	

	OutputLog();
	~OutputLog() = default;

	inline static OutputLog* m_OutputLog = nullptr;
};