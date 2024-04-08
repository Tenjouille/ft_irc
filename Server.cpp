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
	{
		return it->second->getSocket();
	}
	else
	{
		return -1;
	}
}

Client*	Server::getClient(int socket) const
{
	std::map<int, Client*>::const_iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
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
	char msg_to_send[1024];
	int bytes_read;
	int status;

	bytes_read = recv(socket, buffer, BUFSIZ, 0);
	buffer[bytes_read] = '\0';
	std::cout << RED << "'" << buffer << "'" << std::endl;
	for (int j = 0; j <= _fdMax; j++)
	{
		if (FD_ISSET(j, &_allSockets) && j != _socket && j != socket)
		{
			//send(getClientSocket(socket), to_send.c_str(), to_send.length(), 0);
			status = send(j, msg_to_send, strlen(msg_to_send), 0);
		}
	}
	parser(buffer, socket);
	// for (int i = 0; i  1024)
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

void	Server::delClient(int socket)
{
	std::map<int,Client*>::iterator it = _clients.find(socket);
	if (it == _clients.end())
	{
		return;
	}
	else
		_clients.erase(it);
}

void	Server::quitCmd(int socket)
{
	FD_CLR(socket, &_allSockets);
	delClient(socket);
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
			if (i == _socket)
				accept_new_connection();
			else
				read_data_from_socket(i);
		}
	}
}

void    Server::caplsCmd(std::string locate, int socket)
{
    (void)locate;
    if (replyClient(CAP_LS, socket) != static_cast<size_t>(-1))
	{
		getClient(socket)->updateStatus();
	}
}

void    Server::defineCmd(std::string cmd, int start, int it, int socket)
{
	std::string locate;
	locate.append(cmd, start, it - start);
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

void	Server::passCmd(std::string cmd, int socket)
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
		std::string username = getClient(socket)->getUserName();
		replyClient(ERROR_INVPASS(username), socket);
		return ;
	}
	// else
	// 	std::cout << "ALL GOOD SAME PASSWORD" << std::endl;
	getClient(socket)->updateStatus();
}

void	Server::parser(char *buffer, int socket)
{
	std::string cmd = buffer;
	int start = 0;
	std::cout << BLUE << "commande recu par irssi: '" << cmd << "'" << std::endl;
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '\r' && cmd[i + 1] == '\n')
		{
			defineCmd(cmd, start, i, socket);
			i += 2;
			start = i;
		}
	}
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
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
		{
			printf("nickname wrong input");
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		it->second->setNickName(cmd);
		// Attention a si meme nickname ya PROBLEMES ou si user essaye de changer de nickname
		it->second->updateStatus(); //status = 3
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
	{
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
		{
	//		std::cout << "username wrong" << std::endl;
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		it->second->setUserName(cmd);
		it->second->updateStatus();
		if (it->second->getStatus() == 4)
		{
			std::string server_name = "localhost"; // TODO : setup un getter pour le nom de server
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			replyClient(WELCOME_MSG(server_name, username, nickname), socket);
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