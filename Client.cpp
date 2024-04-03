#include "Client.hpp"

Client::Client()
{
    std::cout << "New client" << std::endl;
}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{
    std::cout << "New client" << std::endl;
}
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
// {

// }

// void Client::setUserName() const
// {

// }

// void Client::setName() const
// {

// }

Client::~Client()
{}