#include "Server.hpp"

#include <iostream>
#include <string>
#include <cstring>

bool    Server::isClientOp(std::map<int, Client*> op_list, int socket)
{
    // std::map<int, Client *>::iterator ite = op_list.end();

    for (std::map<int, Client *>::iterator it = op_list.begin(); it != op_list.end(); ++it)
    {
        if (_clients[socket]->getNickName() == it->second->getNickName())
        {
            std::cout << "USER IS AN OPERATOR SO ALL GOOD" << std::endl;
            return (true);
        }
        // ++it;
    }
    return (false);
}

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
            if (it->second->getTopicStatus() == 1 && isClientOp(it->second->getOperatorList(), socket) == false)
            {
                std::cout << YELLOW << "CLIENT IS NOT AN OPERATOR !" << RESET << std::endl;
                // std::string tmp = "#" + channelName; //NEED TO ADD THE #
                std::string err_msg = ERR_CHANOPRIVSNEEDED(_clients[socket]->getNickName(), channelName);
                std::cout << "SENDING : " << err_msg << std::endl;
                replyClient(err_msg, socket);
                return ;
            }
            else
            {
                it->second->setTopic(topicName);
                break;
            }
        }
    }
    send_in_channel(channelName, _clients[socket]->getNickName(), topicName, socket, "topic");
    (void)socket;
}