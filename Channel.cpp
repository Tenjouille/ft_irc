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

void    notOp()
{
    std::cout << "Message d'erreur si client n'est pas operateur" << std::endl;
}

int Channel::getLimit() const
{
    return (_limit);
}

std::string Channel::getKey() const
{
    return _key;
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
    
    // std::string welcome2Channel = "\x1b[1m\x1b[32mWelcome in the #" + _name + " channel.\x1b[0m\nSay hello to :\n";
    // std::cout << YELLOW << "ADDED : " << client->getNickName() << " and socket : " << socket << RESET << std::endl;
    // for(std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
    //     welcome2Channel += "    " + it->second->getNickName() + "\n";
    // if (_clientslst.size() == 0)
    //     welcome2Channel += "your fresh new server (me ^^)!\n";
    // welcome2Channel += "And fear the powerfull judgment of :\n\x1b[31m\x1b[1m";
    // for(std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
    //     welcome2Channel += "    " + it->second->getNickName() + "\n\x1b[0m";
    // // replyClient(welcome2Channel, socket);
    // std::cout << "SENDING : " << SENDINCHANNEL(client->getNickName(), client->getNickName(), welcome2Channel,_name) << std::endl;
    
    _clientslst.insert(std::make_pair(socket, client));
    
    // replyClient(SENDINCHANNEL(client->getNickName(), client->getNickName(), welcome2Channel,_name), client->getSocket());
    printChannelUsers();
}

bool Channel::getInvitOnly() const
{
    return _inviteonly;
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
    std::cout << RED << _clientslst.size() << std::endl;
    for (std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
    {
        std::cout << "Comparing : '" << clientName << "'" << " and : '" << it->second->getNickName() << "'" << std::endl;
        if (it->second->getNickName() == clientName)
        {
            std::cout << BLUE << "CLIENT DEL FROM LIST" << RESET << std::endl;
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