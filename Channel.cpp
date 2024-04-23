#include "Channel.hpp"

Channel::Channel(std::string str)
{
    setName(str);
    _topicStatus = 0;
}

void    notOp()
{
    std::cout << "Message d'erreur si client n'est pas operateur" << std::endl;
}

// void    Channel::kickClient(int socket, std::string name)
// {
//     if (_operators.find(socket) == _operators.end())
//         return (notOp());
//     for(std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
//         if (it->second->getNickName() == name)
//         {
//             delete it->second; // IMPORTANT : clone() les nouveaux clients de _clientslst et _operator !!!
//             _clientslst.erase(it->second->getSocket());
// }

void    Channel::addClient(int socket, Client *client)
{
    std::string welcome2Channel = "Welcome in " + _name + " server.\nSay hello to :\n";
    for(std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
        welcome2Channel += "    " + it->second->getNickName() + "\n";
    if (_clientslst.size() == 0)
        welcome2Channel += "your fresh new server (me ^^)!\n";
    _clientslst.insert(std::make_pair(socket, client));
    welcome2Channel += "And fear the powerfull judgment of :\n";
    for(std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
        welcome2Channel += "    " + it->second->getNickName() + "\n";
    replyClient(welcome2Channel, socket);
}

void Channel::newOperator(int socket, Client *client)
{
    // std::string newSign = "@" + client->getNickName();
    _operators.insert(std::make_pair(socket, client));
    // client->setNickName(newSign);
}

// void    Channel::delClient(int socket)
// {
//     _clientslst.erase(socket);
// }

void    Channel::removeClientFromLst(std::string clientName)
{
        for (std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); ++it)
        {
            if (it->second->getNickName() == clientName)
            {
                _clientslst.erase(it->first);
            }
        }
        for (std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
        {
            if (it->second->getNickName() == clientName)
            {
                _operators.erase(it->first);
            }
        }
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