#include "Channel.hpp"

int    isOneOf(char c, std::string chars)
{
    for(int i = 0; chars[i]; i++)
        if (c == chars[i])
            return (1);
    return (0);
}

std::string	initOptions(std::string cmd)
{
    std::string    ret = "";
    size_t        i = 0;

    if (cmd == "")
        return (NULL);
    while (cmd[i] && cmd[i] != '+' && cmd[i] != '-')
        i++;
    if (cmd[i] != '+' && cmd[i] != '-')
        return ("");
    while (cmd[i] && cmd[i] != '\r' && cmd[i] != ' ')
    {
        if (!isOneOf(cmd[i], "+-itkol"))
            return ("");
        ret += cmd[i++];
    }
    return (ret);
}

std::vector<std::string>	initArgs(std::string cmd)
{
	std::vector<std::string>	ret;
	std::string					node = "";

	if (cmd == "" || (cmd[0] != '+' && cmd[0] != '-'))
		return (ret);
	for (size_t i = 0; cmd[i];)
	{
		if (cmd[i] == '+' || cmd[i] == '-')
			while (cmd[i] && cmd[i] != ' ')
				i++;
		while (cmd[i] == ' ')
			i++;
		if (cmd[i] && cmd[i] != '+' && cmd[i] != '-')
		{
			while (cmd[i] && cmd[i] != ' ')
				node += cmd[i++];
			ret.push_back(node);
			node = "";
		}
	}
	return (ret);
}

void    Channel::defineMode(char sign, char option, std::vector<std::string>& args, int socket)
{
    if (option == 'i')
        return (changeInvit(sign));
    if (option == 't')
        return (changeTopic(sign));
    if (option == 'k')
        return (changeKey(sign, args, socket));
    if (option == 'o')
        return (changeOperator(sign, args, socket));
    if (option == 'l')
        return (changeLimit(sign, args, socket));
    else
        return ;        
}

void    Channel::execMode(std::string options, std::vector<std::string>& args, int socket)
{
    if (options.empty())
        return ;
    char sign = options[0];
    for(size_t i = 0; options[i]; i++)
    {
        if (options[i] == '+' || options[i] == '-')
            sign = options[i];
        else
            defineMode(sign, options[i], args, socket);
    }
}

void    Server::modeCmd(std::string locate, int socket)
{
    std::string cmd = locate.substr(locate.find(' ') + 1);
    (void) socket;
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); it++)
    {
        if (cmd.find(it->first) == 1 && cmd[0] == '#')
        {
            std::string                    options = initOptions(cmd.substr(cmd.find(' ') + 1));
            std::vector<std::string>    args = initArgs(cmd.substr(cmd.find(' ') + 1));
            it->second->execMode(options, args, socket);
            return ;
        }
    }
}

