#include "Server.hpp"

#include <iostream>
#include <string>
#include <cstring>

void Server::topicCmd(std::string locate, int socket)
{
    std::string channelName;
    std::string topicName;

    size_t start = locate.find('#');
    if (start != std::string::npos)
    {
        size_t end = locate.find(' ', start);
        if (end != std::string::npos)
        {
            channelName = locate.substr(start + 1, end - start - 1);
        }
    }
    start = locate.rfind(':');
    if (start != std::string::npos)
    {
        topicName = locate.substr(start + 1);
    }
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if (it->first == channelName)
        {
            it->second->setTopic(topicName);
            break;
        }
    }
    send_in_channel(channelName, _clients[socket]->getNickName(), topicName, socket, "topic");
    (void)socket;
}

bool    Server::isClientOp(std::map<int, Client*> op_list, int socket)
{
    std::map<int, Client *>::iterator it = op_list.begin();
    std::map<int, Client *>::iterator ite = op_list.end();

    while (it != ite)
    {
        if (_clients[socket]->getNickName() == it->second->getNickName())
        {
           // std::cout << "USER IS AN OPERATOR SO ALL GOOD" << std::endl;
            return (true);
        }
        ++it;
    }
    return (false);
}