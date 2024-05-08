#include "Channel.hpp"

int	isOneOf(char c, std::string chars)
{
	for(int i = 0; chars[i]; i++)
		if (c == chars[i])
			return (1);
	return (0);
}

std::string	Server::initOptions(std::string cmd)
{
	std::string	ret = "";
	size_t		i = 0;

	if (cmd == "")
		return ("");
	while (cmd[i] && cmd[i] != '+' && cmd[i] != '-')
		i++;
	if (cmd[i] != '+' && cmd[i] != '-')
		return (""); // 501 ERR_UMODEUNKNOWNFLAG
	while (cmd[i] && cmd[i] != ' ')
	{
		if (!isOneOf(cmd[i], "+-itkol"))
			return (""); // 501 ERR_UMODEUNKNOWNFLAG
		ret += cmd[i++];
	}
	std::cout << YELLOW << "options: [" << ret << "]" << std::endl;
	return (ret);
}

std::string	Server::initArgs(std::string cmd)
{
	std::string					node = "";
	size_t						stop = cmd.find(' ');
	if (cmd == "")
		return ("");
	if (stop == std::string::npos)
		stop = cmd.size();
	for(size_t i = 0; i != stop; i++)
		node += cmd[i];
	std::cout << GREEN << "arg: [" << node << "]" << RESET << std::endl;
	return (node);
}

void	Channel::defineMode(char sign, char option, std::vector<std::string>& args, int socket)
{
	std::cout << "sign = " << sign << " option = " << option << std::endl;
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
		//Error : OPTION NON MANDATORY OU INEXISTANTE !!!
		return ;
}

void	Channel::execMode(std::string options, std::vector<std::string>& args, int socket)
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

size_t	optionNB(std::string options)
{
	size_t  count = 0;

	for(size_t i = 0; i < options.size(); i++)
	{
		if (options[i] == '+' || options[i] == '-')
			continue ;
		count++;
	}
	return (count);
}

int	Server::modeCmd(std::string locate, int socket)
{
	std::cout << "locate : " << locate << std::endl;
	std::string	cmd = locate.substr(locate.find(' ') + 1);
	std::cout << CYAN << cmd << " -> " << _channelLst.size() << RESET << std::endl;
	std::string	channelName = "";
	size_t		end = cmd.find(' ');

	if (end == std::string::npos)
		end = cmd.size();
	channelName.append(cmd, 0, end);
	for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); it++)
	{
		if (cmd.find(it->first) == 1 && cmd[0] == '#' && "#" + it->first == channelName)
		{
			if (locate == "MODE " + channelName)
			{
				std::cout << "C GOOD" << std::endl;
				return 0;
			}
			if (isClientOp(it->second->getOperatorList(), socket) == false)
            {
				std::cout << "ICI LOL" << std::endl;
                std::string tmp = channelName;
                if (tmp[0] == '#')
                    tmp.erase(0, 1);
            	return (replyClient(ERR_CHANOPRIVSNEEDED(_clients[socket]->getNickName(), tmp), socket));
            }
            size_t	nextArg = cmd.find(' ');
			if (nextArg == std::string::npos)
				nextArg = cmd.size() - 1;
			cmd = cmd.substr(nextArg + 1);
			nextArg = cmd.find(' ');
			if (nextArg == std::string::npos)
				nextArg = cmd.size() - 1;
			while (!cmd.empty())
			{
				// AJOUTER UNE CONDITION DE VERIFICATION SI SOCKET EST OPERATOR ICI
				std::string					options = "";
				std::vector<std::string>	args;

				options = initOptions(cmd);
				std::cout << RED << cmd << " -> " << _channelLst.size() << std::endl;
				if (!options.empty())
					cmd = cmd.substr(nextArg + 1);
				else
				{
					std::cout << RED << "babye " << RESET << std::endl;
					break ;
				}
				for(size_t i = 0; i != optionNB(options) && !cmd.empty(); i++)
				{
					args.push_back(initArgs(cmd));
					nextArg = cmd.find(' ');
					if (nextArg == std::string::npos)
						nextArg = cmd.size() - 1;
					cmd = cmd.substr(nextArg + 1);
				}
				it->second->execMode(options, args, socket);
				it->second->infoChannel();
			}
			return (0);
		}
	}
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (cmd.find(it->second->getNickName()) == 0)
			return (0);
	return (replyClient(ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channelName), socket));
}