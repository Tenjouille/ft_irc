#include "Channel.hpp"

Channel::Channel(std::string str)
{
    setName(str);
}

void Channel::addClient(int socket, Client *client)
{
    _clientslst.insert(std::make_pair(socket, client));
}

void Channel::setName(std::string str)
{
    _name = str;
}

std::map<int, Client*> Channel::getClientlst()
{
    return _clientslst;
}

std::string Channel::getName()
{
    return _name;
}