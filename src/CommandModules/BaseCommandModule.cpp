#include "PCH.hpp"
#include "CommandModules/BaseCommandModule.hpp"
#include "Utility/OutputLog.hpp"
#include "FSCLT.hpp"


void BaseCommandModule::SetModuleName(const std::string& ModuleName)
{
	this->m_CommandModuleName = ModuleName;

	IS_TRUE_ASSERT(!m_CommandModuleName.empty(), "MODULE NAME MUST BE SET");
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
	
	IS_TRUE_ASSERT(m_um_CommandDispatch.find(pattern) != m_um_CommandDispatch.end(),
		"There exists already a Command with the same pattern!");

	m_um_CommandDispatch[pattern] = func;
}
bool BaseCommandModule::IsFormattedCorrectly(const std::string& pattern)
{
	if (pattern.empty())
	{
		IS_TRUE_ASSERT(true, "Why is the pattern a empty string?");
		return false;
	}
	else if (HasIllegalStartAndEndCharacter(pattern))
	{
		IS_TRUE_ASSERT(true, "Illegal character at the start and end position found!");
		return false;
	}
	else if (CheckLeadingSpaces(pattern))
	{
		IS_TRUE_ASSERT(true, "There is a space after a space. You cannot have 2 or more leading spaces after a space!");
		return false;
	}
	else if (HasIllegalCharacters(pattern))
	{
		IS_TRUE_ASSERT(true, "Illegal characters found!");
		return false;
	}
	else if (AreBracketsConnected(pattern))
	{
		IS_TRUE_ASSERT(true, "Brackets have to be separated from other words!");
		return false;
	}

	return true;
}
bool BaseCommandModule::AreBracketsConnected(const std::string& pattern)
{
	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == '{')
		{
			if (i - 1 > 0 && pattern[i - 1] != ' ')
				return true;
		}
		else if (pattern[i] == '}')
		{
			if (i + 1 < pattern.size() && pattern[i + 1] != ' ')
				return true;
		}
	}
	return false;
}
bool BaseCommandModule::HasIllegalStartAndEndCharacter(const std::string& pattern)
{
	bool HasIllegalCharacter =
		//CheckSpace
		pattern.front() == ' ' ||
		pattern.back() == ' ' ||
		//Check brackets
		pattern.front() == '{' ||
		pattern.front() == '}';

	return HasIllegalCharacter;
}
bool BaseCommandModule::HasIllegalCharacters(const std::string& pattern)
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
bool BaseCommandModule::CheckLeadingSpaces(const std::string& pattern)
{
	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == ' ')
		{
			if (i < pattern.size() && pattern[i + 1] == ' ')
				return true;
		}
	}
	return false;
}
std::vector<std::string> BaseCommandModule::ResolvePattern(const std::string& pattern)
{
	IS_TRUE_ASSERT(!IsFormattedCorrectly(pattern), "The Command isnt formatted correctly");

	std::vector<std::string> patternVec;
		
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
			IS_TRUE_ASSERT(true, "Unskipped closed parenthesis detected. Did you forget to add an open parenthesis?");
			break;

		default:
			temp = ConsumeCmdIdentifier(pattern, i);
			break;

		}
		
		if (!temp.empty())
		{
			patternVec.push_back(temp);
			i += temp.size()-1;
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
	IS_TRUE_ASSERT(true, "Unexpected '{' found");

	return std::string();
}
