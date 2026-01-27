#pragma once

#define DECLARE_SUB_COMMAND(pattern, CommandModule, func) DeclareSubCommand(pattern, std::bind(&CommandModule::func, this, std::placeholders::_1, std::placeholders::_2))

enum EFLAG_PARAM : uint8_t
{
	//No flag set
	ENONE = 0x01,
	//recursuve flag
	EFLAG_RECURSIVE = 0x02,
	//local flag
	EFLAG_LOC = 0x04,
	//info flag
	EFLAG_INFO = 0x08,
	//info flag
	EFLAG_LIST = 0x10,
};

class BaseCommandModule
{
public:
	BaseCommandModule(const std::string& m_CommandModuleName) : m_CommandModuleName(m_CommandModuleName) { }
	virtual ~BaseCommandModule() = default;

public:
	void SetModuleName(const std::string& ModuleName);

	void InitializeArguments(const std::vector<std::string>& args);

	inline std::string GetCommandModuleName() const { return m_CommandModuleName; }

	virtual void PrintUsageInfo() const = 0;

	bool Execute();

protected:
	std::vector<std::string> m_v_args;

	void ReportInvalidCommand();

	void DeclareSubCommand(const std::string& pattern,
		std::function<bool(const std::vector<std::string>&, uint8_t UserArgs)> func);

private:
	//resolve pattern relateds
	std::vector<std::string> ResolvePattern(const std::string& pattern);	
	std::string ConsumeCmdIdentifier(const std::string& str, size_t pos);
	std::string RetrievePatternParam(const std::string& str, size_t pos);

	//returns true if the command pattern is formatted correctly
	bool IsFormattedCorrectly(const std::string& pattern);

	//returns true if illegal characters were found
	bool HasIllegalCharacters(const std::string& pattern);

	//returns true if a invalid character was found at start and end position
	bool HasIllegalStartAndEndCharacter(const std::string& pattern);

	//returns true if useless spaces were found
	bool CheckLeadingSpaces(const std::string& pattern);

	//returns true if the bracket is not connected to another word
	bool AreBracketsConnected(const std::string& pattern);

	//Characters that are not allowed to use in a pattern
	std::vector<char> m_v_IllegalCharacters{
		'\t',
		'\n',
		'\r',
		'\033',
		'\v',
		'\a',
		'*',
		'/',
		'-',
		'+',
	};
private:
	std::string m_CommandModuleName;

	std::unordered_map<std::string, std::function<bool(const std::vector<std::string>&, uint8_t UserArgs)>> m_um_CommandDispatch;
};