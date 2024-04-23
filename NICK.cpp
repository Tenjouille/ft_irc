#include "Server.hpp"

bool	Server::checkNickName(std::string to_check, int socket)
{
	std::string tmp_name;
	
	std::map<int, Client*>::iterator it = _clients.begin();
	int i = 0;
	while (it != _clients.end())
	{
		tmp_name = it->second->getNickName();
		if (to_check == tmp_name && socket != it->first)
		{
			return (false);
		}
		++it;
		i++;
	}
	return (true);
}

void	Server::nickCmd(std::string str, int socket)
{
	std::string cmd = str.substr(str.find(' ') + 1);	
	if (cmd[0] == '\0')
	{
		replyClient(ERR_NONICKNAMEGIVE(getClient(socket)->getNickName()), socket);
		return;
	}
	if (_clients[socket]->do_we_set_or_not() == true)
	{
		return ;
	}
	if (cmd.length() > 9)
	{
		replyClient(ERRONEUSNICKNAME_ERR(getClient(socket)->getNickName()), socket);
		return;
	}	
	for (int i = 0; cmd[i]; i++)
	{
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9') || cmd[i] == '_'))
		{
			replyClient(ERRONEUSNICKNAME_ERR(getClient(socket)->getNickName()), socket);
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		// Attention a si meme nickname ya PROBLEMES ou si user essaye de changer de nickname
		std::string nickname = cmd;
		std::cout << nickname << std::endl;
		if (checkNickName(nickname, socket) == false && _clients[socket]->getStatus() >= 4)
		{
			replyClient(NICKNAMEINUSE_ERR(nickname), socket);
			return;
		}
		else
		{
			it->second->setNickName(cmd);
			it->second->updateStatus();
		}
		if (it->second->getStatus() >= 4)
		{
			it->second->setNickName(cmd);
			std::cout << it->second->getNickName() << std::endl;
			std::string server_name = "localhost";
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			replyClient(WELCOME_MSG(server_name, nickname, username), socket);
		}
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}