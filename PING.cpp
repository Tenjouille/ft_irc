#include "Server.hpp"

void	Server::pingCmd(std::string cmd, int socket)
{
	std::string arg = cmd.substr(cmd.find(' ') + 1);
	if (arg[0] == '\0')
	{
		replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "PING"), socket);
		return;
	}
	std::string arg2 = "localhost";
	sleep(1);
	replyClient(DEFAULTPONG(arg2), socket);
}
