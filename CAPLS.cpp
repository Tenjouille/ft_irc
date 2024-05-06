#include "Server.hpp"

void	Server::caplsCmd(std::string locate, int socket)
{
	(void)locate;
	if (replyClient(CAP_LS(), socket) != static_cast<size_t>(-1))
	{
		if (_clients[socket]->getStatus() == 0)
			getClient(socket)->updateStatus(1);
	}
}