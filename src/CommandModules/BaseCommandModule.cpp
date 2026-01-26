#include "PCH.hpp"
#include "CommandModules/BaseCommandModule.hpp"
#include "Utility/OutputLog.hpp"
#include "FSCLT.hpp"


void BaseCommandModule::SetModuleName(const std::string& ModuleName)
{
	this->m_CommandModuleName = ModuleName;

	REQUIRED_ASSERT(m_CommandModuleName.empty(), "MODULE NAME MUST BE SET");
}

bool BaseCommandModule::Execute()
{
	return true;
}

void BaseCommandModule::InitializeArguments(const std::vector<std::string>& args)
{
	this->m_v_args = args;
}

void BaseCommandModule::ReportInvalidCommand()
{
	
}
void BaseCommandModule::DeclareSubCommand(const std::string& pattern,
	std::function<bool(const std::vector<std::string>&, uint8_t UserArgs)> func)
{
	ResolvePattern(pattern);
	
	REQUIRED_ASSERT(m_um_CommandDispatch.find(pattern) != m_um_CommandDispatch.end(),
		"There exists already a Command with the same pattern!");

	m_um_CommandDispatch[pattern] = func;
}
std::vector<std::string> BaseCommandModule::ResolvePattern(const std::string& pattern)
{
	std::vector<std::string> patternVec;
	if (CheckIllegalCharacters(pattern))
	{
		REQUIRED_ASSERT(true, "Illegal characters found!");
		return std::vector<std::string>();
	}
		
	for (size_t i = 0; i < pattern.size(); i++)
	{
		std::string temp;
		
		switch (pattern[i])
		{

		case '{':
			temp = RetrievePatternParam(pattern, i);
			break;

		case '}':
			//This character is usually skipped if a '{' were detected
			REQUIRED_ASSERT(true, "Unskipped closed parenthesis detected. Did you forget to add an open parenthesis?");
			break;

		default:
			temp = ConsumeCmdIdentifier(pattern, i);
			break;

		}
		
		if (!temp.empty())
		{
			patternVec.push_back(temp);
			i += temp.size();
			OutputLog::Get().SendOutput("Current proccesed string: " + temp);
		}
		
	}

	return patternVec;
}
std::string BaseCommandModule::ConsumeCmdIdentifier(const std::string& str, size_t pos)
{
	std::string temp;
	size_t CurPos = pos;

	for (CurPos = pos; CurPos < str.size(); CurPos++)
	{
		if (str[CurPos] != ' ')
		{
			temp.push_back(str[CurPos]);
		}
		else
			break;
		
	}
	return temp;
}
std::string BaseCommandModule::RetrievePatternParam(const std::string& str, size_t pos)
{
	//Skip '{'
	for (size_t CurPos = pos + 1; CurPos < str.size(); CurPos++)
	{
		if (str[CurPos] == ' ' || str[CurPos] == '{')
			break;

		else if (str[CurPos] == '}')
			return std::string(str.begin() + pos, str.begin() + CurPos+1);
	}
	REQUIRED_ASSERT(true, "Unexpected '{' found");

	return std::string();
}
bool BaseCommandModule::CheckIllegalCharacters(const std::string& pattern)
{
	for (const auto& patt : pattern)
	{
		for (const auto& c : m_v_IllegalCharacters)
		{
			if (patt == c)
				return true;
		}
	}
	return false;
}