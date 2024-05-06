#include "Server.hpp"

void    Server::delFromChannels(int socket)
{
    std::map<std::string, Channel *>::iterator it = _channelLst.begin();
    std::map<std::string, Channel *>::iterator ite = _channelLst.end();

    while (it != ite)
    {
        std::cout << "REMOVING " << _clients[socket]->getNickName() << " from " << it->second->getName() << std::endl;
        it->second->removeClientFromLst(_clients[socket]->getNickName());
        ++it;
    }
}


void    Server::delClient(int socket)
{
    delFromChannels(socket);
    if (_clients.find(socket) != _clients.end())
    {
        // if (_nb_clients > 0)
        // {
            _clients[socket]->setNickName("");
            _clients[socket]->setTempBuffer("", 1);
            _nb_clients--;
            delete _clients[socket];
            _clients.erase(socket);
            FD_CLR(socket, &_allSockets);
	        _fdMax--;
        // }
    }
    else
        return ;
}

void    Server::quitCmd(int socket)
{
    // std::cout << "On detruit le client" << std::endl;
    // std::vector<std::string>::iterator it  = _clients[socket]->getChannel().begin();
    // std::vector<std::string>::iterator ite = _clients[socket]->getChannel().end();
    // while (it != ite)
    // {
    //     std::cout << "looking at " << *it << std::endl;
    //     if (it != _clients[socket]->getChannel().end())
    //     {
    //         std::string channelName = *it;
    //         std::map<std::string, Channel*>::iterator it_c = _channelLst.begin();
    //         for (;it_c != _channelLst.end(); ++it_c)
    //         {
    //             if (it_c->first == channelName)
    //             {  
    //                 std::cout << "removing from " << it_c->first << ":" << _clients[socket]->getNickName() << std::endl;
    //                 it_c->second->removeClientFromLst(_clients[socket]->getNickName());
    //             }
    //         }
    //     }
    //     ++it;
    // }
    std::cout << "Pas rentre for" << std::endl;
    delClient(socket);
}