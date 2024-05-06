#include "Client.hpp"

Client::Client()
{
	_nickcount = 0;
	_connection_status = 0;
	_dont_set_user = false;
	_is_connected = false;
	_sent = false;
	_skip = false;
}

void	Client::setSent(bool to_set)
{
	_sent = to_set;
}

bool	Client::getSent() const
{
	return (_sent);
}

void	Client::setSkip(bool to_set)
{
	if (to_set == true)
		_skip = true;
	else
		_skip = false;
}

bool	Client::getSkip() const
{
	return (_sent);
}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{}

void	Client::change_connected()
{
	_is_connected = true;
}

bool	Client::getConnectedStatus() const
{
	return (_is_connected);
}

std::string	Client::getTempBuffer()
{
	return _tempBuffer;
}

void  Client::setTempBuffer(std::string str, int flag)
{
	if (flag == 0)
		_tempBuffer += str;
	else if (flag == 1)
		_tempBuffer = "";
}

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

void	Client::printChannels()
{
	std::vector<std::string>::iterator it = _channel.begin();
	std::vector<std::string>::iterator itend = _channel.end();
	
	while (it != itend)
	{
		std::cout << *it << std::endl;
		++it;
	}
}

void	Client::delChannel(std::string channelName)
{
	for (std::vector<std::string>::iterator it = _channel.begin(); it != _channel.end(); ++it)
	{
		if(*it == channelName)
		{
			it = _channel.erase(it);
		}
	}
}

/* ==== GETTERS ====*/

std::string Client::getNickName() const
{
	return _nickname;
}

void    Client::updateStatus(int to_set)
{
    // _connection_status++;
	_connection_status = to_set;
	std::cout << "STATUS UPDATED TO : " << _connection_status << std::endl;
}

std::string Client::getUserName() const
{
	return _username;
}

std::string Client::getName() const
{
	return _name;
}

std::vector<std::string> Client::getChannel()
{
	return _channel;
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

void    Client::addChannel(std::string channelname_to_add)
{
	_channel.push_back(channelname_to_add);
	std::cout << "ADDED " << channelname_to_add << " into client's vector" << std::endl;
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

void	Client::setConnectedStatus(bool to_set)
{
	_connection_status = to_set;
	std::cout << "CONNECTION STATUS SET TO " << to_set << std::endl;
}

void	Client::ClearNick()
{
	_nickname.clear();
}

/* ==== DESTRUCTEUR ==== */

Client::~Client()
{}