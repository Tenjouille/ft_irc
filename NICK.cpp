#include "Server.hpp"

// void	Server::nickCmd(std::string str, int socket)
// {
// 	std::cout << MAGENTA << "str = '" << str << "'" << std::endl;
// 	std::string cmd = str.substr(str.find(' ') + 1);
// 	std::cout << RED << "cmd = '" << cmd << "'" << std::endl;
// 	if (cmd.length() > 9)
// 	{
// 		printf("trop de char pour nick");
// 		return;
// 	}	
// 	for (int i = 0; cmd[i]; i++)
// 	{
// 		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
// 		{
// 			printf("nickname wrong input");
// 			return;
// 		}
// 	}
// 	std::map<int, Client*>::iterator it;
//     for (it = _clients.begin(); it != _clients.end(); ++it)
//     {
//         if (it->second->getNickName() == cmd)
//         {
// 			_nickcount++;
// 			std::cout << "ICI" << std::endl;
// 			replyClient(NICKNAMEINUSE_ERR(cmd), socket);
//             //getClient(socket)->gettmpNick() = cmd;
// 			return;
//         }
//     }
//     it = _clients.find(socket);
//     if (it != _clients.end())
//     {
// 		it->second->setNickName(cmd);
// 		it->second->updateStatus(); //status = 3
// 	}
// 	else
// 		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
// }

//si j ai set un nickname et que je veux connecter qqun avec le meme nickname ca part en couille
void	Server::nickCmd(std::string str, int socket)
{
	std::string cmd = str.substr(str.find(' ') + 1);
	if (cmd.length() > 9)
	{
		printf("trop de char pour nick");
		return;
	}	
	for (int i = 0; cmd[i]; i++)
	{
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9') || cmd[i] == '_'))
		{
			printf("nickname wrong input");
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		//std::cout << YELLOW << "cmd = '" << it->second->getNickName() << "'" << std::endl; 
		// Attention a si meme nickname ya PROBLEMES ou si user essaye de changer de nickname
		std::string nickname = cmd;
		if (checkNickName(nickname, socket) == false)
		{
			replyClient(NICKNAMEINUSE_ERR(nickname), socket);
		}
		else
		{
			it->second->setNickName(cmd);
			// std::cout << BLUE << "Set : " << cmd << RESET << std::endl;
			it->second->updateStatus();
			// std::cout << GREEN << "UPDATING :" << it->second->getStatus() << RESET << std::endl;
		}
		if (it->second->getStatus() >= 4)
		{
			// std::cout << BLUE << "SETTING : " << cmd << RESET << std::endl;
			it->second->setNickName(cmd);
			//std::cout << it->second->getNickName() << std::endl;
			std::string server_name = "localhost"; // TODO : setup un getter pour le nom de server
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			replyClient(WELCOME_MSG(server_name, nickname, username), socket);
		}
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}

bool	Server::checkNickName(std::string to_check, int socket)
{
	std::string tmp_name;
	(void) socket;
	
	std::map<int, Client*>::iterator it = _clients.begin();
	int i = 0;
	while (it != _clients.end())
	{
		tmp_name = it->second->getNickName();
		//peut etre check la socket aussi pour s'assurer qu'on compare
		//pas avec lui meme ?
		if (to_check == tmp_name && socket != it->first)
		{
			// std::cout << RED << "COMPARING : " << to_check << " ET " << tmp_name << RESET << std::endl;
			// std::cout << RED << NICKNAMEINUSE_ERR(test) << std::endl;
			return (false);
		}
		++it;
		i++;
	}
	return (true);
}