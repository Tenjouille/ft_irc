#include "Server.hpp"

void	Server::nickCmd(std::string str, int socket)
{
	std::cout << MAGENTA << "str = '" << str << "'" << std::endl;
	std::string cmd = str.substr(str.find(' ') + 1);
	std::cout << RED << "cmd = '" << cmd << "'" << std::endl;
	if (cmd.length() > 9)
	{
		printf("trop de char pour nick");
		return;
	}	
	for (int i = 0; cmd[i]; i++)
	{
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
		{
			printf("nickname wrong input");
			return;
		}
	}
	std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getNickName() == cmd)
        {
			_nickcount++;
			std::cout << "ICI" << std::endl;
			replyClient(NICKNAMEINUSE_ERR(cmd), socket);
            //getClient(socket)->gettmpNick() = cmd;
			return;
        }
    }
    it = _clients.find(socket);
    if (it != _clients.end())
    {
		it->second->setNickName(cmd);
		it->second->updateStatus(); //status = 3
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}

