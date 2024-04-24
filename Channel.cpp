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

int Channel::getLimit() const
{
    return (_limit);
}

bool Channel::getInvitOnly() const
{
    return _inviteonly;
}

std::string Channel::getKey() const
{
    return _key;
}

std::map<int, Client*>  Channel::getOperatorList() const
{
    return (_operators);
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

void    Channel::printChannelUsers()
{
    // std::map<int, Client *liste> client_list = _clientslst;
    std::map<int, Client *>::iterator it = _clientslst.begin();
    std::map<int, Client *>::iterator ite = _clientslst.end();

    int i = 0;

    std::cout << BLUE << "[ CLIENT LISTE ]" << RESET << std::endl;
    while (it != ite)
    {
        
        std::cout << "Client[" << i << "] :" << it->second->getNickName() << std::endl;
        i++;
        ++it;
    }
}

void    Channel::addClient(int socket, Client *client)
{
    
    std::cout << YELLOW << "ADDED : " << client->getNickName() << " and socket : " << socket << RESET << std::endl;
    std::string welcome2Channel = "Welcome in " + _name + " channel.\nSay hello to :\n";
    for(std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
        welcome2Channel += "    " + it->second->getNickName() + "\n";
    if (_clientslst.size() == 0)
        welcome2Channel += "your fresh new server (me ^^)!\n";
    _clientslst.insert(std::make_pair(socket, client));
    welcome2Channel += "And fear the powerfull judgment of :\n";
    for(std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
        welcome2Channel += "    " + it->second->getNickName() + "\n";
    replyClient(welcome2Channel, socket);
    printChannelUsers();
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