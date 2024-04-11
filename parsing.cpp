#include "Server.hpp"

void	Server::parser(char *buffer, int socket)
{
	if (!buffer)
		return ;
	std::string cmd = buffer;
	int start = 0;
	std::cout << BLUE << "commande recu par irssi: '" << cmd << "'" << std::endl;
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '\r' && cmd[i + 1] == '\n')
		{
			defineCmd(cmd, start, i, socket);
			i ++;
			start = i + 1;
		}
	}
}

std::string		kindOptions(std::string cmd, char sign)
{
	std::string	ret = "";

	ret[0] = sign;
	if (cmd == "")
		return (NULL);
	for (size_t i = 0; i < cmd.size(); i++)
		if (cmd[i] == sign)
			while(cmd[i] != ' ' && cmd[i])
				ret += cmd[i++];
	return (ret);
}

std::string	Server::defineOptions(std::string cmd)
{
	std::string	ret = kindOptions(cmd, '+') + kindOptions(cmd, '-');
	std::cout << YELLOW << ret << std::endl;
	return (ret);
}

void	Server::defineCmd(std::string str, int start, int it, int socket)
{
	std::string locate;
	std::string	options;
	std::string	args;
	std::string	cmd;
	locate.append(str, start, it - start);
	cmd.append(str, start, str.find(' '));
	options.append(defineOptions(locate));
	args.append(defineArgs(locate));
	std::cout << "CMD[" << cmd << "] OPTIONS[" << options << "] ARGS[" << args << "]" << std::endl;
	std::cout << GREEN << "apres decoupage, commande = '" << locate << "'" << std::endl; 
	if (locate.find("NICK") == 0)
	{
		std::cout << WHITE << "passe dans la fonction nick" << std::endl;
		nickCmd (locate, socket);
	}
	else if (locate.find("CAP LS") == 0)
	{
		std::cout << WHITE << "passe dans la fonction capls" << std::endl;
		caplsCmd(locate, socket);
	}
	else if (locate.find("USER") == 0)
	{
		userCmd(locate, socket);
		std::cout << WHITE << "passe dans la fonction user" << std::endl;
	}
	// else if (locate.find("MODE") == 0)
	// 	std::cout << WHITE << "passe dans la fonction mode" << std::endl;
	else if (locate.find("PASS") == 0)
	{
		std::cout << WHITE << "passe dans la fonction pass" << std::endl;
		passCmd(locate, socket);
	}
	else if (locate.find("JOIN") == 0)
	{
		std::cout << WHITE << "passe dans la fonction join" << std::endl;
		joinCmd(locate, socket);
	}
	else if (locate.find("QUIT") == 0)
	{
		quitCmd(socket);
		std::cout << WHITE << "passe dans la fonction quit" << std::endl;
	}
	else if (locate.find("PING") == 0)
	{
		// pingCmd(locate, socket);
		std::cout << WHITE << "passe dans la fonction ping" << std::endl;
	}
	// else if (locate.find("PRIVMSG") == 0)
	// 	std::cout << "!!!PRIVMSG COMMAND!!!" << std::endl;
	// else if (locate.find("MSG") == 0)
	// 	std::cout << "!!!MSG COMMAND!!!" << std::endl;
	// else if (locate.find("KICK") == 0)
	// 	std::cout << "!!!KICK COMMAND!!!" << std::endl;
	// else if (locate.find("PING") == 0)
	// 	std::cout << "!!!PING COMMAND!!!" << std::endl;
	// else if (locate.find("WHOIS") == 0)
	// 	std::cout << "!!!WHOIS COMMAND!!!" << std::endl;
	// else
	// 	std::cout << "???ERROR UNKNOW COMMAND???" << std::endl;
}
