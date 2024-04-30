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
        // _clients.erase(socket);
        if (_nb_clients > 0)
        {
            _clients[socket]->setNickName("");
            _clients[socket]->setTempBuffer("", 1);
            // _clients[socket]->getTempBuffer().erase();
            delete _clients[socket];
            _nb_clients--;
        }
    }
}

void	Server::quitCmd(int socket)
{
	FD_CLR(socket, &_allSockets);
    std::cout << "On detruit le client" << std::endl;
	delClient(socket);
	_fdMax--;
}