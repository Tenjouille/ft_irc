#include "Server.hpp"

Server::Server(char **av)
{
	size_t i;
	_status = 0;
	_nb_channels = 0;
	_nb_clients = 0;
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

int	Server::getChannelNumber() const
{
	return _nb_channels;
}

int	Server::getClientNumber() const
{
	return _nb_clients;
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

void	Server::WelcomeMsg(std::string channel_name, Client *client, int socket)
{
	// (void) socket;
	Channel	*channel;
	for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        std::string tmp_name = it->first;
        channel = it->second;
        if (tmp_name == channel_name)
		{
            std::map<int, Client*> client_list = channel->getClientlst();
			for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
            {
            	if (it_clt->first != socket)
                {
		 			std::string nickname = client->getNickName();
					// std::string msg_to_send = "\x1b[1m\x1b[32mWelcome in the #" + channel_name + " channel.\x1b[0m";
					std::string msg_to_send = "\x1b[1m\x1b[32mSay Hello to our new member : " + nickname + " !\x1b[0m";
					std::string channel_msg = "\x1b[3m\x1b[36m#" + channel_name;
					std::string msg = SENDINCHANNEL(channel_msg, nickname, msg_to_send, channel_name);
            		replyClient(msg, it_clt->first);
                }
				else
				{
		 			std::string nickname = client->getNickName();
					// std::string msg_to_send = "\x1b[1m\x1b[32mWelcome in the #" + channel_name + " channel.\x1b[0m";
					std::string welcome = "\x1b[1m\x1b[32mBienvenu dans le #" + channel_name + " channel !\x1b[0m";
					std::string cowboy = "\x1b[1m\x1b[5m\x1b[34mFait pas le cowboy et tout ira bien\x1b[0m\n";
					std::string channel_msg = "\x1b[3m\x1b[36m#" + channel_name;
					std::string msg = SENDINCHANNEL(channel_msg, nickname, welcome, channel_name);
            		replyClient(msg, socket);
					msg.clear();
					msg = SENDINCHANNEL(channel_msg, nickname, cowboy, channel_name);
            		replyClient(msg, socket);
				}
            }
			
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
	close(_socket);
}