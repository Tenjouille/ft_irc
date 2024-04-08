#include "Channel.hpp"

Channel::Channel(std::string str)
{
    setName(str);
}

void Channel::addClient(int socket)
{
    
}

void Channel::setName(std::string str)
{
    _name = str;
}

std::string Channel::getName()
{
    return _name;
}