#include "Server.hpp"

void    Server::delClient(int socket)
{
    // for(std::map<std::string, Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    // {
    //     if (it->second->getClientlst().find(socket) != it->second->getClientlst().end())
    //     {
    //         delete it->second->getClientlst()[socket];
    //         it->second->getClientlst().erase(socket);
    //     }
    //     if (it->second->getOperatorList().find(socket) != it->second->getOperatorList().end())
    //     {
    //         delete it->second->getOperatorList()[socket];
    //         it->second->getOperatorList().erase(socket);
    //     }
    // }
    if (_clients.find(socket) == _clients.end())
        return;
    else
    {
        delete _clients[socket];
        _clients.erase(socket);
    }
}

void	Server::quitCmd(int socket)
{
	FD_CLR(socket, &_allSockets);
	_clients.erase(socket);
	delClient(socket);
	_fdMax--;
}