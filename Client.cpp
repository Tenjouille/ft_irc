#include "Client.hpp"

Client::Client()
{
	_nickcount = 0;
	_connection_status = 0;
	_dont_set_user = false;
}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{}

/*//! TMP NOT SURE IF USEFULL !//*/

void	Client::dont_set_user(bool value)
{
	if (value == true)
		_dont_set_user = true;
	else
		_dont_set_user = false;
}

bool	Client::do_we_set_or_not() const
{
	return _dont_set_user;
}


/* ==== GETTERS ====*/

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

struct sockaddr	Client::getClientAddr() const
{
	return (_clientAddr);
}

int Client::getSocket() const
{
	return _socket;
}

int     Client::getStatus()
{
    return _connection_status;
}

/* ==== SETTERS ==== */

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

void    Client::updateStatus()
{
    _connection_status++;
}

/* ==== DESTRUCTEUR ==== */

Client::~Client()
{}