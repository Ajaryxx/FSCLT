#pragma once
#include <iostream>
#include <assert.h>

#define DECLARE_COMMAND_FLAG(CommandPrefix, CommandModule) DelcareCommand<CommandModule>(CommandPrefix)
#define STATIC_IS_BASE_OF(Parent, Child, Message) static_assert(std::is_base_of<Parent, Child>::value, Message)
#define IS_TRUE_ASSERT(Check, Message) assert(!(Check) && Message)