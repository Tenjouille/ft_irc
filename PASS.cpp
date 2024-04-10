#include "Server.hpp"

// void	Server::passCmd(std::string cmd, int socket)
// {
// 	std::string server_pass = getServerPassword();
// 	int i = 0;
// 	while (cmd[i] != '\0' && cmd[i] != ' ')
// 		i++;
// 	i++;
// 	int start = i;
// 	std::string from_client = &cmd[start];
// 	if (from_client.compare(server_pass) != 0)
// 	{
// 		replyClient(ERROR_INVPASS(getClient(socket)->getUserName()), socket);
// 		return ;
// 	}
// 	//replyClient(VALID_PASS(getClient(socket)->getUserName()), socket);
// 	// else
// 	// 	std::cout << "ALL GOOD SAME PASSWORD" << std::endl;
// 	getClient(socket)->updateStatus();
// }

// std::string	Server::getServerPassword() const
// {
// 	return _password;
// }