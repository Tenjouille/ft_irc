#include "Command.hpp"

// Command::Command(char *buffer)
// {

// }


std::string Command::getName() const
{
	return _name;
}

std::vector<std::string> Command::getArgs() const
{
	return _args;
}

std::string Command::getOptions () const
{
	return _options;
}

Command::~Command()
{}