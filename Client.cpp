#include "Client.hpp"

Client::Client()
{
    _nickcount = 0;
}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{}

std::string Client::getNickName() const
{
    return _nickname;
}

std::string Client::getUserName() const
{
    return _username;
}

std::string Client::getName() const
{
    return _name;
}

struct sockaddr Client::getClientAddr() const
{
    return (_clientAddr);
}

// void Client::setNickName() const

void Client::setSocket(int tmp)
{
    _socket = tmp;
}

int Client::getSocket() const
{
    return _socket;
}

// void Client::userCmd(std::string str)
// {
//  //   Parameters: <username> <hostname> <servername> :<realname>
// }

// void Client::joinCmd(std::string str)
// {

// }

void Client::setNickName(std::string str)
{
    _nickname = str;
}

void    Client::setStatus(int i)
{
    _status += i;
}

int     Client::getStatus()
{
    return _status;
}
// void Client::setUserName() const
// {

// }

// void Client::setName() const
// {

// }

Client::~Client()
{}