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

std::string intToString(int value)
{
    std::string result;
    bool negative = false;
    
    if (value < 0) {
        negative = true;
        value = -value;
    }
    
    do {
        result = char('0' + value % 10) + result;
        value /= 10;
    } while (value != 0);

    if (negative) {
        result = "-" + result;
    }
    
    return result;
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
	for (int i = 0; cmd[i]; i++)
	{
		if (!(((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9') || cmd[i] == '_')) || (cmd.length() > 9))
		{
			std::string username;
			if (_clients[socket]->getUserName().empty())
				username = "Error";
			else
				username = _clients[socket]->getUserName();
			std::string msg = ERR_ERRONEUSNICKNAME(username, cmd);
			std::cout << "SENDING : " << msg << std::endl;
			replyClient(msg, socket);
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		std::string nickname = cmd;
		std::cout << nickname << std::endl;
		if (checkNickName(nickname, socket) == false)
		{
			_clients[socket]->setSkip(true);
			std::cout << "LA :" << _clients[socket]->getSkip() << std::endl;
			replyClient(NICKNAMEINUSE_ERR(nickname), socket);
			return ;
		}
		else
		{
			if (_clients[socket]->getConnectedStatus() == false)
			{
				it->second->setNickName(cmd);
				if (_clients[socket]->getStatus() == 2 || _clients[socket]->getStatus() == 3)
				{
					if (_clients[socket]->getStatus() == 3)
						it->second->updateStatus(4);
					else
						it->second->updateStatus(3);
					std::cout << "PASSE ICI" << std::endl;
				}
			}
			else
			{
				// ":nickname NICK new_nickname\r\n"
				std::string old = _clients[socket]->getNickName();
				std::string msg = ":" + old + " NICK " + nickname + "\r\n";
				std::cout << "SENDING ICI : " << msg << std::endl;
				replyClient(msg, socket);
				it->second->setNickName(cmd);
				return ;
			}
		}
		if (it->second->getStatus() >= 4)
		{
			it->second->setNickName(cmd);
			std::cout << it->second->getNickName() << std::endl;
			std::string server_name = "localhost";
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			if (_clients[socket]->getConnectedStatus() == false)
			{
				replyClient(WELCOME_MSG(server_name, nickname, username), socket);
				_clients[socket]->change_connected();
				startingMsg(socket);
			}
		}
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}