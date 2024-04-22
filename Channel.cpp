#include "Channel.hpp"

Channel::Channel(std::string str)
{
    setName(str);
    _topicStatus = 0;
}

void Channel::addClient(int socket, Client *client)
{
    _clientslst.insert(std::make_pair(socket, client));
}

// void    Channel::delClient(int socket)
// {
//     _clientslst.erase(socket);
// }

void Channel::setName(std::string str)
{
    _name = str;
}

std::map<int, Client*> Channel::getClientlst()
{
    return _clientslst;
}

void Channel::setTopic(std::string str)
{
    _topic = str;
    _topicStatus = 1;
}

int Channel::getTopicStatus()
{
    return _topicStatus;
}

std::string Channel::getTopic()
{
    return _topic;
}

std::string Channel::getName()
{
    return _name;
}