#include "Server.hpp"

void	Server::quitCmd(int socket)
{
	FD_CLR(socket, &_allSockets);
	_clients.erase(socket);
	// delClient(socket);
	_fdMax--;
}