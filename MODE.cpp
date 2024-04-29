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
        return (""); // 501 ERR_UMODEUNKNOWNFLAG
    while (cmd[i] && cmd[i] != '\r' && cmd[i] != ' ')
    {
        if (!isOneOf(cmd[i], "+-itkol"))
            return (""); // 501 ERR_UMODEUNKNOWNFLAG
        ret += cmd[i++];
    }
    std::cout << YELLOW << "options: [" << ret << "]" << std::endl;
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
			std::cout << RED << "HEEEEEEERE !!!!!" << std::endl;
			while (cmd[i] && cmd[i] != ' ')
				node += cmd[i++];
			std::cout << node << std::endl;
			ret.push_back(node);
			node = "";
			std::cout << WHITE << ret[ret.size() - 1] << std::endl;
		}
	}
	std::cout << GREEN << "ARGS :" << std::endl;
	for(size_t it = 0; it < ret.size(); it++)
		std::cout << "[" << ret[it] << "]" << std::endl;
	return (ret);
}

void	Channel::defineMode(char sign, char option, std::vector<std::string>& args)
{
	if (option == 'i')
		return (changeInvit(sign));
	if (option == 't')
		return (changeTopic(sign));
	if (option == 'k')
		return (changeKey(sign, args));
	if (option == 'o')
		return (changeOperator(sign, args));
	if (option == 'l')
		return (changeLimit(sign, args));
	else
		//Error : OPTION NON MANDATORY OU INEXISTANTE !!!
		return ;		
}

void	Channel::execMode(std::string options, std::vector<std::string>& args)
{
	char sign = options[0];

	for(size_t i = 0; options[i]; i++)
	{
		if (options[i] == '+' || options[i] == '-')
			sign = options[i];
		else
			defineMode(sign, options[i], args);
	}
}

void	Channel::infoChannel()
{
	std::cout << MAGENTA << "ABOUT " << getName() << ":" << std::endl;
	std::cout << "Topic : [" << _topic << "]" << std::endl;
	std::cout << "Key : [" << _key << "]" << std::endl;
	std::cout << "Limit : [" << _limit << "]" << std::endl;
	std::cout << "Inviteonly : [" << _inviteonly << "]" << std::endl;
	std::cout << "Clients :" << std::endl;
	for (std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
		std::cout << "[" << it->second->getNickName() << "]" << std::endl;
	std::cout << "Operators :" << std::endl;
	for (std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
		std::cout << "[" << it->second->getNickName() << "]" << std::endl;
}

void	Server::modeCmd(std::string locate, int socket)
{
	std::cout << CYAN << locate << std::endl;
	std::string cmd = locate.substr(locate.find(' ') + 1);
	(void) socket;
	for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); it++)
	{
		if (cmd.find(it->first) == 1 && cmd[0] == '#')
		{
			// AJOUTER UNE CONDITION DE VERIFICATION SI SOCKET EST OPERATOR ICI
			std::string					options = initOptions(cmd.substr(cmd.find(' ') + 1));
			std::vector<std::string>	args = initArgs(cmd.substr(cmd.find(' ') + 1));

			it->second->execMode(options, args);
			it->second->infoChannel();
			return ;
		}
	}
	//Error : NOM DU CHANNEL ERRONE !!!
}
