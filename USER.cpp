#include "Server.hpp"

bool 	Server::userCheckArgs(std::string str)
{
	size_t i = 0;
	int	found = 0;
	size_t len = str.length();
	while (str[i] != '\r' && i < len)
	{
		if (str[i] == ' ')
			found++;
		i++;
	}
	if (found > 2)
		return (true);
	return (false);
}

void	Server::userCmd(std::string str, int socket)
{
	if (userCheckArgs(str) == false)
	{
		replyClient(ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "USER"), socket);
		return;
	}
	std::cout << YELLOW << "STR : " << str << RESET << std::endl;
	std::string cmd = str.substr(str.find(' ') + 1);
	cmd = cmd.substr(0, cmd.find(' '));
	if (getClient(socket)->getStatus() == 4)
	{
		replyClient(ALREADYREGISTERED(getClient(socket)->getNickName()), socket);
		return;
	}
	for (int i = 0; cmd[i]; i++)
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
			return;
	if (_clients[socket])
	{
		_clients[socket]->setUserName(cmd);
		_clients[socket]->updateStatus();
		if (_clients[socket]->getStatus() == 4)
		{
			std::string server_name = "localhost";
			std::string username = _clients[socket]->getUserName();
			std::string nickname = _clients[socket]->getNickName();
			if (checkNickName(nickname, socket) == false)
			{
				std::string tmp_username = _clients[socket]->getUserName();
				replyClient(NICKNAMEINUSE_ERR(tmp_username), socket);
			}
			if (_clients[socket]->getConnectedStatus() == false)
			{
				replyClient(WELCOME_MSG(server_name, nickname, username), socket);
				_clients[socket]->change_connected(); //true
				startingMsg(socket);
			}
		}
		else
		{
			return ;
		}
	}
	else
		std::cerr << "Client not found for socket: " << socket << std::endl;
}