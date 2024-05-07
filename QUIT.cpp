#include "Server.hpp"

void    Server::delFromChannels(int socket)
{
    std::map<std::string, Channel *>::iterator it = _channelLst.begin();
    std::map<std::string, Channel *>::iterator ite = _channelLst.end();

    while (it != ite)
    {
        it->second->removeClientFromLst(_clients[socket]->getNickName());
        ++it;
    }
}


void    Server::delClient(int socket)
{
    delFromChannels(socket);
    if (_clients.find(socket) != _clients.end())
    {
            _clients[socket]->setNickName("");
            _clients[socket]->setTempBuffer("", 1);
            _nb_clients--;
            delete _clients[socket];
            _clients.erase(socket);
            FD_CLR(socket, &_allSockets);
	        _fdMax--;
    }
    else
        return ;
}

void    Server::quitCmd(int socket)
{
    delClient(socket);
}