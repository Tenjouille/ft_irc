#include "Client.hpp"

Client::Client()
{

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