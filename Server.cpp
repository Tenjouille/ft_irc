#include "Server.hpp"

Server::Server(char **av)
{
	size_t i;
	_status = 0;
	for(i = 0; isdigit(av[1][i]) != 0; i++)
		continue ;
	_password = av[2];
 //   std::cout << "Password Stocke : " << _password << std::endl;
	if (i == strlen(av[1]))
	{
		int port = atoi(av[1]);
		_socket = socket(AF_INET, SOCK_STREAM, 0);
		_serverAddress.sin_family = AF_INET;
		_serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
		_serverAddress.sin_port = htons(port);
		bind(_socket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
	}
	else
		std::cout << "invalid port format" << std::endl;
}

int	Server::getSocket() const
{
	return _socket;
}

struct sockaddr_in	Server::getServerAddress() const
{
	return _serverAddress;
}

int	Server::getClientSocket(int socket) const
{
	std::map<int, Client*>::const_iterator it = _clients.find(socket);
	if (it != _clients.end())
		return it->second->getSocket();
	else
		return -1;
}

Client*	Server::getClient(int socket) const
{
	std::map<int, Client*>::const_iterator it = _clients.find(socket);
	if (it != _clients.end())
		return it->second;
	else
		return NULL;
}

void	Server::setClientSocket(int tmp)
{
	Client* client = new Client();
	client->setSocket(tmp);
	_clients.insert(std::make_pair(tmp, client));
}

void	Server::read_data_from_socket(int socket)
{
	char buffer[1024];
	// char msg_to_send[1024];
	int bytes_read;
	// int status;

	bytes_read = recv(socket, buffer, 1024, 0);
	buffer[bytes_read] = '\0';
	std::cout << RED << "'" << buffer << "'" << std::endl;
	if (buffer[0] == '\0')
		return ;
	for (int j = 0; j <= _fdMax; j++)
	{
		if (FD_ISSET(j, &_allSockets) && j != _socket && j != socket)
		{
			//send(getClientSocket(socket), to_send.c_str(), to_send.length(), 0);
			// status = send(j, msg_to_send, strlen(msg_to_send), 0);
			// if (status == -1)
			// 	quitCmd(j);
			std::cout << "COUCOU" << std::endl;
		}
	}
	parser(buffer, socket);
	buffer[0] = '\0';
}

void	Server::accept_new_connection()
{
	int client_fd;

	client_fd = accept(_socket, NULL, NULL);
	setClientSocket(client_fd);
	FD_SET(client_fd, &_allSockets);
	if (client_fd > _fdMax)
		_fdMax = client_fd;
}

// void	Server::delClient(int socket)
// {
// 	std::map<int,Client*>::iterator it = _clients.find(socket);
// 	if (it == _clients.end())
// 	{
// 		return;
// 	}
// 	else
// 		_clients.erase(it);
// }

void	Server::quitCmd(int socket)
{
	FD_CLR(socket, &_allSockets);
	_clients.erase(socket);
	// delClient(socket);
	_fdMax--;
}

void	Server::loop()
{
	struct timeval timer;
	int status;
	while (true)
	{
		_readFds = _allSockets;
		timer.tv_sec = 2;
		timer.tv_usec = 0;
		status = select(_fdMax + 1, &_readFds, NULL, NULL, &timer);
		for (int i = 0; i <= _fdMax; i++)
		{
			if (FD_ISSET(i, &_readFds) != 1)
				continue ;
			// if (FD_ISSET(i, &_readFds) == -1)
			// 	return;
			if (i == _socket)
				accept_new_connection();
			else
				read_data_from_socket(i);
		}
	}
}

void	Server::caplsCmd(std::string locate, int socket)
{
	(void)locate;
	if (replyClient(CAP_LS, socket) != static_cast<size_t>(-1))
		getClient(socket)->updateStatus();
}

void	Server::pingCmd(std::string cmd, int socket)
{
	(void) cmd;
	std::string servername = "localhost";
	sleep(1);
    replyClient(DEFAULTPONG(servername), socket);
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
	// args.append(defineArgs(locate, cmd.size()));
	std::cout << GREEN << "============== NEW COMMAND ==============" << RESET << std::endl;
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
		std::cout << WHITE << "passe dans la fonction user" << std::endl;
        userCmd(locate, socket);
    }
    // else if (locate.find("MODE") == 0)
	// 	std::cout << WHITE << "passe dans la fonction mode" << std::endl;
	else if (locate.find("PASS") == 0)
	{
		std::cout << WHITE << "passe dans la fonction pass" << std::endl;
		passCmd(str, locate, socket);
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
		std::cout << WHITE << "passe dans la fonction ping" << std::endl;
		pingCmd(locate, socket);
	}
	else if (locate.find("PRIVMSG") == 0)
	{
		std::cout << WHITE << "passe dans la fonction privmsg" << std::endl;
		msgCmd(locate, socket);
	}
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

void Server::joinCmd(std::string locate, int socket)
{
	(void)socket;
	(void)locate;
	std::cout << RED << "ici" << std::endl;
}

std::string	Server::getServerPassword() const
{
	return _password;
}

std::string Server::getUsernameFormNick(std::string to_parse)
{
	std::cout << "TO PARSE : '" << to_parse << "'" << std::endl;
	std::string tmp_name = to_parse;

	size_t pos = tmp_name.find("NICK ");
	if (pos != std::string::npos)
	{
		pos += 5;
		std::string name;

		while (pos < tmp_name.size() && tmp_name[pos] != '\r' && tmp_name[pos] != '\n')
		{
			name += tmp_name[pos];
			pos++;
		}
		return (name);
	}
	else
	{
		std::cout << "COULDNT FIND IN THE BUFFER" << std::endl;
		return ("<default_nickname>");
	}
}

void	Server::passCmd(std::string str, std::string cmd, int socket)
{
	std::string server_pass = getServerPassword();
	int i = 0;
	while (cmd[i] != '\0' && cmd[i] != ' ')
		i++;
	i++;
	int start = i;
	std::string from_client = &cmd[start];
	if (from_client.compare(server_pass) != 0)
	{
		std::string username = getUsernameFormNick(str);
		std::cout << "Sending Username : " << username << std::endl;
		replyClient(ERROR_INVPASS(username), socket);
		//ct ici
		return ;
	}
	// else
	// 	std::cout << "ALL GOOD SAME PASSWORD" << std::endl;
	getClient(socket)->updateStatus();
}

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
			std::cout << RED << "COMPARING : " << to_check << " ET " << tmp_name << RESET << std::endl;
			// std::cout << RED << NICKNAMEINUSE_ERR(test) << std::endl;
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
		if (checkNickName(nickname, socket) == false && _clients[socket]->getStatus() >= 4)
		{
			replyClient(NICKNAMEINUSE_ERR(nickname), socket);
		}
		else
		{
			it->second->setNickName(cmd);
			std::cout << BLUE << "Set : " << cmd << RESET << std::endl;
			it->second->updateStatus();
			std::cout << GREEN << "UPDATING :" << it->second->getStatus() << RESET << std::endl;
		}
		if (it->second->getStatus() >= 4)
		{
			std::cout << BLUE << "SETTING : " << cmd << RESET << std::endl;
			it->second->setNickName(cmd);
			std::cout << it->second->getNickName() << std::endl;
			std::string server_name = "localhost"; // TODO : setup un getter pour le nom de server
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			replyClient(WELCOME_MSG(server_name, nickname, username), socket);
		}
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}

Server::~Server()
{
	close(_socket);
}

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
}

void	Server::setfdMax(int socket)
{
	_fdMax = socket;
}

fd_set& Server::getallSockets()
{
	return _allSockets;
}

fd_set& Server::getreadFds()
{
	return _readFds;
}
int Server::getfdMax()
{
	return _fdMax;
}

void	Server::closeSockets()
{
	for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		close(it->first);
}
