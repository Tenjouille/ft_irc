#include "Server.hpp"

void	Server::pingCmd(std::string cmd, int socket)
{
	std::string arg = cmd.substr(cmd.find(' ') + 1);
	std::cout << "CMD : " << cmd << std::endl;
	std::cout << arg << std::endl;
	std::string arg2 = "\0";
	sleep(1);
	std::cout << "Sending : '" << PONG(arg, arg2) << "'" << std::endl;
	// replyClient(PONG(arg, arg2), socket);
	replyClient(PONG(arg2, arg2), socket);
	// std::string test_msg = "PONGGGG";
	// replyClient(test_msg, socket);
}