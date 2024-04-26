#include "Server.hpp"

Server::Server(char **av)
{
<<<<<<< HEAD
    // for (int i = 0; av[i] != NULL; i++)
    //     std::cout << av[i] << std::endl;
    size_t i;
    for(i = 0; isdigit(av[1][i]) != 0; i++)
    {}
    if (i == strlen(av[1]))
    {
        int port = atoi(av[1]);
        //set le password du serveur
        setServerPassword(av[2]);

        /*etablit une interface de communication entre le serveur et un client
        (renvoie un descripteur)
        AF_INET = domaine d'adresses, ici: IPv4
        SOCK_STREAM = type de socket
        0 = protocole a utiliser, 0 pcq on s en ballec, le systeme gere
        */
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        //checker que socket renvoie pas - 1;
        /*
        _serverAddress contient les informations du serveur
        sin.family specifie le type d'adresse
        inet_addr specifie l'adresse  IP du serveur, ici localhost
        sin.port specifie le port en format reseau
        */
        _serverAddress.sin_family = AF_INET;
        _serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        _serverAddress.sin_port = htons(port);
        /*
        bind lie la socket à l’addresse IP de mon ordi
        */
        bind(_socket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
        /*
        listen ecoute via la socket pour détecter des demandes de connexion
        5 est un entier qui represente le nb de connexions autorisees dans la file d attente (nb a changer)
        */
        listen(_socket, 5);
        //checker que ca renvoit pas -1
    }
    else
        std::cout << "invalid port format" << std::endl;
=======
	size_t i;
	_status = 0;
	_nb_channels = 0;
	for(i = 0; isdigit(av[1][i]) != 0; i++)
		continue ;
	_password = av[2];
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
>>>>>>> main
}

Channel* Server::getChannelFromName(std::string name)
{
	std::map<std::string, Channel*>::iterator it = _channelLst.begin();
	std::map<std::string, Channel*>::iterator ite = _channelLst.end();

	while (it != ite)
	{
		std::string tmp_channel_name = it->first;
		std::cout << "COMPARING : '" << tmp_channel_name << "' and : '" << name << "'" << std::endl;
		if (tmp_channel_name == name)
		{
			std::cout << "CHANNEL FOUND !" << std::endl;
			return (it->second);
		}
		++it;
	}
	return (NULL);
}

std::map<std::string, Channel*> Server::getChannelListe() const
{
	return _channelLst;
}


int	Server::getSocket() const
{
	return _socket;
}

void	Server::read_data_from_socket(int socket)
{
//	std::cout << "read data" << std::endl;
	char buffer[1024];
	// char msg_to_send[1024];
	int bytes_read;
	//int status;
	bytes_read = recv(socket, buffer, 1024, 0);
	buffer[bytes_read] = '\0';
	//std::cout << RED << "'" << buffer << "'" << std::endl;
	if (buffer[0] == '\0')
		return ;
	for (int j = 0; j <= _fdMax; j++)
	{
		// if (FD_ISSET(j, &_allSockets) && j != _socket && j != socket)
		// {
		// 	//send(getClientSocket(socket), to_send.c_str(), to_send.length(), 0);
		// 	// status = send(j, msg_to_send, strlen(msg_to_send), 0);
		// 	// if (status == -1)
		// 	// 	quitCmd(j);
		// }
	}
	//test ctrl+D
	std::string cmd1 = buffer;
	if (cmd1[cmd1.size() - 2] != '\r' && cmd1[cmd1.size() - 1] != '\n')
		std::cout << "cmd1 = '" << cmd1 << "'" << std::endl;
	//std::cout << GREEN << "HERE" << RESET << std::endl;
	getClient(socket)->setTempBuffer(static_cast<char*>(buffer), 0);
	//std::cout << "buffer client = '" << getClient(socket)->getTempBuffer() << "'" << std::endl;
	std::string cmd = getClient(socket)->getTempBuffer();
	if (cmd[cmd.size() - 2] == '\r' && cmd[cmd.size() - 1] == '\n')
	{
		//std::cout << "cmd = '" << cmd << "'" << std::endl;
		parser(getClient(socket)->getTempBuffer(), socket);
		if (getClient(socket))
			getClient(socket)->setTempBuffer("", 1);
	}
	// for (int i = 0; i  1024)
	buffer[0] = '\0';
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

void	Server::setfdMax(int socket)
{
	_fdMax = socket;
}

void	Server::setClientSocket(int tmp)
{
	Client* client = new Client();
	client->setSocket(tmp);
	_clients.insert(std::make_pair(tmp, client));
}

/* //? ==== MAIN FUNCTIONS ==== ?// */

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


void	Server::accept_new_connection()
{
		std::cout << "accept" << std::endl;

	int client_fd;

	client_fd = accept(_socket, NULL, NULL);
	setClientSocket(client_fd);
	FD_SET(client_fd, &_allSockets);
	if (client_fd > _fdMax)
		_fdMax = client_fd;
}

void	Server::defineCmd(std::string str, int start, int it, int socket)
{
	std::string locate;
	std::string	options;
	std::string	args;
	std::string	cmd;
	locate.append(str, start, it - start);
	cmd.append(str, start, str.find(' '));
	// options.append(defineOptions(locate));
	// args.append(defineArgs(locate, cmd.size()));
	// std::cout << GREEN << "============== NEW COMMAND ==============" << RESET << std::endl;
	//std::cout << GREEN << "apres decoupage, commande = '" << locate << "'" << std::endl; 
	if (locate.find("NICK") == 0)
		nickCmd (locate, socket);
	else if (locate.find("CAP LS") == 0)
		caplsCmd(locate, socket);
	else if (locate.find("USER") == 0)
        userCmd(locate, socket);
    else if (locate.find("MODE") == 0)
		modeCmd(locate, socket);
	else if (locate.find("PASS") == 0)
		passCmd(cmd, locate, socket);
	else if (locate.find("JOIN") == 0)
	{
//		std::cout << WHITE << "passe dans la fonction join avec string '" << locate << "'" << std::endl;
		joinCmd(locate, socket);
	}
	else if (locate.find("QUIT") == 0)
		quitCmd(socket);
	else if (locate.find("PING") == 0)
		pingCmd(locate, socket);
	else if (locate.find("PRIVMSG") == 0)
	{
		msgCmd(locate, socket);
	}
	else if (locate.find("INVITE") == 0)
	{
	//	std::cout << "LETS GO INVITE BOYS" << std::endl;
		inviteCmd(locate, socket);
	}
	else if (locate.find("TOPIC") == 0)
	{
		topicCmd(locate, socket);
	}
	else if (locate.find("KICK") == 0)
	{
	//	std::cout << "!!!KICK COMMAND!!!" << std::endl;
		kickCmd(locate, socket);
	}
}

void	Server::parser(std::string cmd, int socket)
{
	if (cmd.empty())
		return ;
	int start = 0;
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

void	Server::closeSockets()
{
	for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		close(it->first);
}

Server::~Server()
{
<<<<<<< HEAD
    close(_socket);
}

void    Server::setServerPassword(std::string to_set)
{
    _server_password = to_set;
}

void    Server::passCmd(std::string cmd)
{
    std::cout << "CMD :: " << cmd << std::endl;
    std::cout << "Password du serveur : " << getServerPassword() << std::endl;

    std::string server_pass = getServerPassword();

    //skip PASS
    int i = 0;
    while (cmd[i] != '\0' && cmd[i] != ' ')
        i++;
    i++;
    //now at the start of the entered password
    int start = i;
    std::string from_client = &cmd[start];
    std::cout << "TMP FROM STaRT : " << from_client << std::endl;
    std::cout << "AFTER SKIPPING PASS COMMAND : '" << &cmd[start] << "'" << std::endl;
    if (from_client.compare(server_pass) != 0)
    {
        std::cout << RED << "ERRROOOOOR NOT THE SAME PASSWORD !!!" << RESET << std::endl;
        std::cout << RED << server_pass << " vs " << from_client << RESET << std::endl;
        sendToClient("Erreur 464 :Password incorrect\n");
        //close la connection du coup
        return ;
    }
    else
    {
        //continue le processus de connection
        std::cout << GREEN << "ALL GOOD SAME PASSWORD" << RESET << std::endl;
    }
}

ssize_t Server::sendToClient(std::string to_send)
{
    //vide for now a remplir temporairement en attendant le nick et tout
    std::cout << "CLIENT GETNAME VALUE : " << _client.getName() << std::endl;
    std::string buffer = _client.getName() + to_send;

    ssize_t j = send(getClientSocket(), buffer.c_str(), buffer.length(), 0);
    return (j);
}

std::string Server::getServerPassword() const
{
    return _server_password;
=======
	close(_socket);
>>>>>>> main
}