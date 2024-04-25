#include "Channel.hpp"

int	isOneOf(char c, std::string chars)
{
	for(int i = 0; chars[i]; i++)
		if (c == chars[i])
			return (1);
	return (0);
}

std::string		initOptions(std::string cmd, size_t &i)
{
	std::string	ret = "";

	
	if (cmd == "")
		return ("");
	while(cmd[i] && cmd[i] != '+' && cmd[i] != '-')
		i += 1;
	if (cmd[i] != '+' && cmd[i] != '-')
		return ("");
	std::cout << cmd[i] << std::endl;
	std::cout << cmd << " [" << i << "] " << std::endl;
	while(cmd[i] && cmd[i] != '\r' && cmd[i] != ' ')
	{
		std::cout << RED << cmd[i];
		if (!isOneOf(cmd[i], "+-itkol"))
			return (""); //Error : INVALID OPTION
		ret += cmd[i++];
	}
	std::cout << std::endl;
	std::cout << YELLOW << "options: [" << ret << "]" << std::endl;
	return (ret);
}

std::vector<std::string>	initArgs(std::string cmd, int count, size_t &i)
{
	std::vector<std::string>	ret;
	std::string					node = "";
	int							loop = 0;

	if (cmd == "")
		return (ret);
	while (cmd[i] && loop++ < count)
	{
		std::cout << RED << "HEEEEEEERE !!!!!" << std::endl;
		while (cmd[i] && cmd[i] != ' ')
			node += cmd[i++];
		while (cmd[i] == ' ')
			i += 1;
		std::cout << node << std::endl;
		ret.push_back(node);
		node = "";
		std::cout << WHITE << ret[ret.size() - 1] << std::endl;
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
	std::cout << "Topic : [" << _topic << "]";
	if (_topicStatus)
		std::cout << " on" << std::endl;
	else
		std::cout << " off" << std::endl;
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

int	optionNb(std::string options)
{
	int count = 0;

	if (!options.size())
		return (count);
	for(size_t i = 0; options[i]; i++)
	{
		if (options[i] == '+' || options[i] == '-')
			continue ;
		else
			count++;
	}
	return (count);
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
			if (it->second->getOperators().find(socket) == it->second->getOperators().end())
				return ; // Error : LE CLIENT N'EST PAS UN OPERATEUR !!!
			size_t						i = it->first.size() + 1;
			std::string					options = initOptions(cmd, i);
			while (cmd[i] == ' ')
				i++;
			std::cout << WHITE << "Valeur de i = " << i << " Valeur  de count = " << optionNb(options) << std::endl; 
			std::vector<std::string>	args = initArgs(cmd, optionNb(options), i);

			std::cout << YELLOW << "Executed options :";
			it->second->execMode(options, args);
			std::cout << RESET << std::endl;
			it->second->infoChannel();
			return ;
		}
	}
	std::cout << WHITE << "HEEEEEEERE" << std::endl;
	//Error : NOM DU CHANNEL ERRONE !!!
}
