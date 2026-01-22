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
	OutputLog::Get().PrintList(ResolvePattern(pattern), "Pattern");
}
std::vector<std::string> BaseCommandModule::ResolvePattern(const std::string& pattern)
{
	std::vector<std::string> patternVec;
	
	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == '{')
		{
			std::string str = RetrievePatternParam(pattern, i);
			patternVec.push_back(str);
			i += str.size();
		}
		else if (pattern[i] != ' ')
		{
			std::string temp = ConsumeCmdIdentifier(pattern, i);
			patternVec.push_back(temp);
			i += temp.size();
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
		{
			break;
		}

	}
	return temp;
}
std::string BaseCommandModule::RetrievePatternParam(const std::string& str, size_t pos)
{
	size_t CurPos = pos;
	for (CurPos = pos; CurPos < str.size(); CurPos++)
	{
		if (str[CurPos] == ' ')
			break;

		else if (str[CurPos] == '}')
		{
			return std::string(str.begin() + pos, str.begin() + CurPos+1);
		}
			
	}
	REQUIRED_ASSERT(1, "End of string reached! There is no closing }");

	return std::string();
}