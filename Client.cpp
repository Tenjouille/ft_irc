#include "Client.hpp"

Client::Client()
{
	_nickcount = 0;
	_connection_status = 0;
}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{}

std::string Client::getNickName() const
{
	return _nickname;
}

void    Client::updateStatus()
{
    _connection_status++;
}

std::string Client::getUserName() const
{
	return _username;
}

std::string Client::getName() const
{
	return _name;
}

int     Client::getStatus()
{
    return _connection_status;
}

int Client::getSocket() const
{
	return _socket;
}

std::vector<std::string> Client::getChannel()
{
	return _channel;
}

struct sockaddr Client::getClientAddr() const
{
	return (_clientAddr);
}

void Client::setSocket(int tmp)
{
	_socket = tmp;
}


void Client::setNickName(std::string str)
{
	_nickname = str;
}

void Client::setUserName(std::string str)
{
    _username = str;
}

Client::~Client()
{}