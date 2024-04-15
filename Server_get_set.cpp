#include "Server.hpp"

int	Server::getSocket() const
{
	return _socket;
}

struct sockaddr_in	Server::getServerAddress() const
{
	return _serverAddress;
}

fd_set& Server::getallSockets()
{
	return _allSockets;
}

fd_set& Server::getreadFds()
{
	return _readFds;
}

std::string	Server::getServerPassword() const
{
	return _password;
}

std::string Server::getUsernameFormNick(std::string to_parse)
{
	// std::cout << "TO PARSE : '" << to_parse << "'" << std::endl;
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
		return NULL;
	}
}

int Server::getfdMax()
{
	return _fdMax;
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