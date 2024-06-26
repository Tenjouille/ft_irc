#include "Server.hpp"

std::string	Server::getServerPassword() const
{
	return _password;
}

std::string Server::getUsernameFormNick(std::string to_parse)
{
	std::string tmp_name = to_parse;

	size_t pos = tmp_name.find("NICK ");
	if (pos != std::string::npos)
	{
		pos += 5;
		std::string name;
		while (pos < tmp_name.size() && tmp_name[pos] != '\r' && tmp_name[pos] != '\n')
		{
			name += tmp_name[pos];
			pos++;
		}
		return (name);
	}
	else
	{
		std::cout << "COULDNT FIND IN THE BUFFER" << std::endl;
		return ("<default_nickname>");
	}
}

void	Server::passCmd(std::string str, std::string cmd, int socket)
{
	std::string server_pass = getServerPassword();
	int i = 0;
	if (getClient(socket)->getStatus() == 4)
	{
		replyClient(ALREADYREGISTERED(getClient(socket)->getNickName()), socket);
		return;
	}
	while (cmd[i] != '\0' && cmd[i] != ' ')
		i++;
	i++;
	if (cmd == "PASS" || cmd == "PASS ")
	{
		std::string dn = "<default_nickname>";
		replyClient(ERR_NEEDMOREPARAMS(dn, "PASS"), socket);
		return;
	}
	int start = i;
	std::string from_client = &cmd[start];
	if (from_client.compare(server_pass) != 0)
	{
		std::string username = getUsernameFormNick(str);
		_clients[socket]->dont_set_user(true);
		replyClient(ERROR_INVPASS(username), socket);
		return ;
	}
	_clients[socket]->dont_set_user(false);
	getClient(socket)->updateStatus();
}
