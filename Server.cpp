#include "Server.hpp"

Server::Server(char **av)
{
	size_t i;
	_flag = 0;
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

void	Server::setClientSocket(int tmp)
{
	Client* client = new Client();
	client->setSocket(tmp);
	_clients.insert(std::make_pair(tmp, client));
}

void	Server::read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket)
{
	char buffer[1024];
	char msg_to_send[1024];
	int bytes_read;
	int status;

	bytes_read = recv(socket, buffer, BUFSIZ, 0);
	for (int j = 0; j <= fd_max; j++)
	{
		if (FD_ISSET(j, all_sockets) && j != server_socket && j != socket)
		{
			//send(getClientSocket(socket), to_send.c_str(), to_send.length(), 0);
			status = send(j, msg_to_send, strlen(msg_to_send), 0);
		}
	}
	parser(buffer, socket);
}

void	Server::accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max)
{
	int client_fd;

	client_fd = accept(server_socket, NULL, NULL);
	setClientSocket(client_fd);
	FD_SET(client_fd, all_sockets);
	if (client_fd > *fd_max)
		*fd_max = client_fd;
		 std::string s = ":localhost 001 uaupetit :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
		size_t j = send(getClientSocket(client_fd), s.c_str(), s.length(), 0);
   // sendToClient("Erreur 464 :Password incorrect\r\n", socket);
	(void) j;
} 


void	Server::loop_bis(fd_set all_sockets, fd_set read_fds, int fd_max)
{
	struct timeval timer;
	int status;
	while (true)
	{
		read_fds = all_sockets;
		timer.tv_sec = 2;
		timer.tv_usec = 0;
		status = select(fd_max + 1, &read_fds, NULL, NULL, &timer);
		for (int i = 0; i <= fd_max; i++)
		{
			if (FD_ISSET(i, &read_fds) != 1)
				continue ;
			if (i == _socket)
				accept_new_connection(_socket, &all_sockets, &fd_max);
			else
				read_data_from_socket(i, &all_sockets, fd_max, _socket);
		}
	}
}

ssize_t	Server::sendToClient(std::string to_send, int socket)
{
	ssize_t j = send(getClientSocket(socket), to_send.c_str(), to_send.length(), 0);
	// j = 0;
	// if (_flag == 0)
	// {
	
	//	 _flag = 1;
	// }
	return (j);
}

void    Server::caplsCmd(std::string locate, int socket)
{
    (void)locate;
    sendToClient("CAP_ACK LS\r\n", socket);
}

void    Server::defineCmd(std::string cmd, int start, int it, int socket)
{
	std::string locate;

	locate.append(cmd, start, it - start);
	if (locate.find("NICK") == 0)
	{
		std::cout << "!!!NICK COMMAND!!!" << std::endl;
		nickCmd (locate, socket);
	}
	else if (locate.find("USER") == 0)
    {
        userCmd(locate, socket);
		std::cout << "!!!USER COMMAND!!!" << std::endl;
    }
    else if (locate.find("MODE") == 0)
		std::cout << "!!!MODE COMMAND!!!" << std::endl;
	else if (locate.find("PASS") == 0)
	{
		std::cout << "!!!PASS COMMAND!!!" << std::endl;
		passCmd(locate, socket);
	}
	else if (locate.find("JOIN") == 0)
		std::cout << "!!!JOIN COMMAND!!!" << std::endl;
	else if (locate.find("PRIVMSG") == 0)
		std::cout << "!!!PRIVMSG COMMAND!!!" << std::endl;
	else if (locate.find("MSG") == 0)
		std::cout << "!!!MSG COMMAND!!!" << std::endl;
	else if (locate.find("KICK") == 0)
		std::cout << "!!!KICK COMMAND!!!" << std::endl;
	else if (locate.find("PING") == 0)
		std::cout << "!!!PING COMMAND!!!" << std::endl;
	else if (locate.find("WHOIS") == 0)
		std::cout << "!!!WHOIS COMMAND!!!" << std::endl;
	else
		std::cout << "???ERROR UNKNOW COMMAND???" << std::endl;
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
		sendToClient("uaupetit :Password incorrect\r\n", socket);
		return ;
	}
	else
		std::cout << "ALL GOOD SAME PASSWORD" << std::endl;
}

void	Server::parser(char *buffer, int socket)
{
	std::string cmd = buffer;
	int start = 0;
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
	}
	else
		std::cerr << "Client not found for socket: " << socket << std::endl;
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
			std::cout << "username wrong" << std::endl;
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
		it->second->setUserName(cmd);
	else
		std::cerr << "Client not found for socket: " << socket << std::endl;
}