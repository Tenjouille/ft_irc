#include "Server.hpp"

void	Server::pingCmd(std::string cmd, int socket)
{
	std::cout << "cmd = " << cmd << std::endl;
	std::string arg = cmd.substr(cmd.find(' ') + 1);
	if (arg[0] == '\0')
	{
		replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "PING"), socket);
		return;
	}
	std::cout << "arg = '" << arg << "'" << std::endl;
	std::string arg2 = "localhost";
	sleep(1);
	//std::cout << "Sending : '" << PONG(arg, arg2) << "'" << std::endl;
	// replyClient(PONG(arg, arg2), socket);
	replyClient(DEFAULTPONG(arg2), socket);
	//	replyClient(PONG(arg2), socket);

	// std::string test_msg = "PONGGGG";
	// replyClient(test_msg, socket);
}
