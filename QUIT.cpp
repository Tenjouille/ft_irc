#include "Server.hpp"

void    Server::delClient(int socket)
{
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

void	Server::quitCmd(int socket)
{
    std::cout << "On detruit le client" << std::endl;
	delClient(socket);
}