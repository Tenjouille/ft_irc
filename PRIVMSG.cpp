#include "Server.hpp"

int Server::getSocketFromUser(std::string to_find)
{
    int socket = -666;

	std::map<int,Client*>::iterator it = _clients.begin();

    if (it != _clients.end())
    {
        while (it != _clients.end())
        {
            if (it->second->getNickName() == to_find)
            {
                socket = it->first;
                std::cout << "Socket to send to : " << socket << std::endl;
            }
            ++it;
        }
        if (socket == -666)
            std::cout << RED << "ERROR SOCKET NOT FOUND" << RESET << std::endl;
    }
    else
    {
        std::cerr << RED << "ERROR D'ITERATOR" << RESET << std::endl;
    }
    return (socket);
}

void Server::msgCmd(std::string locate, int socket)
{
    std::string user;
    std::string msg;
	std::cout << RED << "locate = " << locate << std::endl;
	//string = "PRIVMSG uaupetit :salut ca va";
	size_t start = locate.find("PRIVMSG ");
	std::cout << GREEN << "start = " << start << std::endl;
	if (start != std::string::npos)
	{
        start += 8;
		size_t end = locate.find(' ', start);
		if (end != std::string::npos) {
            user = '\0';
            msg = '\0';
            user = locate.substr(start, end - start); //destinataire
            msg = locate.substr(end + 2); // +2 pour ignorer ": "
            user[user.length()] = '\0';
            msg[msg.length()] = '\0';
            std::cout << MAGENTA "User: '" << user << "'" << std::endl;
            std::cout << MAGENTA "Message: '" << msg << "'" << std::endl;
        }
		else
		{
            std::cerr << "Erreur: Aucun espace trouvé après le nom d'utilisateur" << std::endl;
        }
    }
	else
	{
        std::cerr << "Erreur: Chaîne ne contient pas 'PRIVMSG '" << std::endl;
    }
    int socket_to_send_to = getSocketFromUser(user);
    
	std::map<int,Client*>::iterator it = _clients.find(socket);

    std::string sender = it->second->getNickName(); 
    std::string servername = "localhost";
    std::string msg_to_send = MSG_TO_SEND(user, servername, sender, msg);
    std::cout << MSG_TO_SEND(user, servername, sender, msg) << std::endl;
    replyClient(msg_to_send, socket_to_send_to);
	// (void)socket;
}
