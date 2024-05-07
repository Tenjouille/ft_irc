#include "Channel.hpp"

Channel::Channel(std::string str)
{
    _name = "";
    _topic = "";
    _key = "";
    _limit = 0;
    _inviteonly = 0;
    setName(str);
    _topicStatus = 0;
}

Channel::~Channel()
{
}

int Channel::getLimit() const
{
    return (_limit);
}

std::string Channel::getKey() const
{
    return _key;
}

void    Channel::printChannelUsers()
{
    std::map<int, Client *>::iterator it = _clientslst.begin();
    std::map<int, Client *>::iterator ite = _clientslst.end();

    int i = 0;
    while (it != ite)
    {
        i++;
        ++it;
    }
}

void    Channel::addClient(int socket, Client *client)
{
    _clientslst.insert(std::make_pair(socket, client));
    printChannelUsers();
}

bool Channel::getInvitOnly() const
{
    return _inviteonly;
}

void Channel::newOperator(int socket, Client *client)
{
    _operators.insert(std::make_pair(socket, client));
}

void    Channel::removeClientFromLst(std::string clientName)
{
    for (std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
    {
        if (it->second->getNickName() == clientName)
        {
            _clientslst.erase(it->first);
            break ;
        }
    }
    for (std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
    {
        if (it->second->getNickName() == clientName)
        {
            _operators.erase(it->first);
            break ;
        }
    }
}

std::map<int, Client*>  Channel::getOperatorList() const
{
    return (_operators);
}

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