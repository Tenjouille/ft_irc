#include "Server.hpp"
/*
void	Server::userCmd(std::string str, int socket)
{
	std::string cmd = str.substr(str.find(' ') + 1);
	cmd = cmd.substr(0, cmd.find(' '));
	for (int i = 0; cmd[i]; i++)
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
			return;
	if (_clients[socket])
	{
		_clients[socket]->setUserName(cmd);
		_clients[socket]->updateStatus();
		if (_clients[socket]->getStatus() == 4)
		{
			std::string server_name = "localhost"; // TODO : setup un getter pour le nom de server
			std::string username = _clients[socket]->getUserName();
			std::string nickname = _clients[socket]->getNickName();
			if (checkNickName(nickname, socket) == false)
			{
				std::string tmp_username = _clients[socket]->getUserName();
				replyClient(NICKNAMEINUSE_ERR(tmp_username), socket);
			}
			replyClient(WELCOME_MSG(server_name, nickname, username), socket);

			std::cout << "sent machin" << std::endl;
		}
		else
		{
			std::cout << "SETUP UsERNAME ET STATUS UPDATED" << std::endl;
			std::cout << _clients[socket]->getStatus() << std::endl;
			return ;
		}
	}
	else
		std::cerr << "Client not found for socket: " << socket << std::endl;
}*/

void	Server::userCmd(std::string str, int socket)
{
	std::string cmd = str.substr(str.find(' ') + 1);
	cmd = cmd.substr(0, cmd.find(' '));
	for (int i = 0; cmd[i]; i++)
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
			return;
	if (_clients[socket])
	{
		_clients[socket]->setUserName(cmd);
		_clients[socket]->updateStatus();
		if (_clients[socket]->getStatus() == 4)
		{
			std::string server_name = "localhost"; // TODO : setup un getter pour le nom de server
			std::string username = _clients[socket]->getUserName();
			std::string nickname = _clients[socket]->getNickName();
			if (checkNickName(nickname, socket) == false)
			{
				std::string tmp_username = _clients[socket]->getUserName();
				replyClient(NICKNAMEINUSE_ERR(tmp_username), socket);
			}
			replyClient(WELCOME_MSG(server_name, nickname, username), socket);

			// std::cout << "sent machin" << std::endl;
		}
		else
		{
		//	std::cout << "SETUP UsERNAME ET STATUS UPDATED" << std::endl;
			return ;
		}
	}
	else
		std::cerr << "Client not found for socket: " << socket << std::endl;
}