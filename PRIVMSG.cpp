#include "Server.hpp"

void Server::send_in_channel(std::string user, std::string s_nick, std::string msg_to_send, int socket, std::string macro)
{
    std::string final_user = user;
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        std::string channel_name = it->first;
        Channel *channel = it->second;
        if (channel_name == user)
        {
            std::map<int, Client*> client_list = channel->getClientlst();
            if (macro == "send")
            {
                for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
                {
                    if (it_clt->first != socket)
                    {
                        std::string msg = SENDINCHANNEL(s_nick, it_clt->second->getNickName(), msg_to_send, user);
                        replyClient(msg, it_clt->first);
                    }
                }
            }
            else if (macro == "topic")
            {
                for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
                {
                    std::string msg = TOPIC(it_clt->second->getNickName(), user, msg_to_send);
                    replyClient(msg, it_clt->first);
                }
            }
        }
    }
}

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
              //  std::cout << "Socket to send to : " << socket << std::endl;
                return (socket);
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

int     Server::isUser(std::string str)
{
    	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (it->second->getNickName() == str)
            {
                return 0;
            }
        }
        return 1;
}

void Server::msgCmd(std::string locate, int socket)
{
	int is_channel = 0;
    std::string user;
    std::string msg;
    
    size_t ping_pos = locate.find("PING");
    if (ping_pos != std::string::npos)
    {
        std::string dest;
        std::string token;
        size_t privmsg_pos = locate.find("PRIVMSG");
        if (privmsg_pos != std::string::npos)
        {
            size_t dest_start = privmsg_pos + 8;
            size_t dest_end = locate.find(" ", dest_start);
            if (dest_end != std::string::npos)
            {
                dest = locate.substr(dest_start, dest_end - dest_start);
            }
            else 
            {
                dest = locate.substr(dest_start);
                if (dest.empty())
                {
                    replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "JOIN"), socket);
                    return;
                }
            }
            if (isUser(dest) == 1)
            {
                replyClient(ERR_NOSUCHNICK(getClient(socket)->getNickName(), dest), socket);
                return;
            }
            std::cout << GREEN << "dest = " << dest << RESET << std::endl;
        }
        size_t token_start = ping_pos + 5;
        size_t token_end = locate.find("\r", token_start);
        if (token_end != std::string::npos)
        {
            token = locate.substr(token_start, token_end - token_start);
        }
        else
        {
            token = locate.substr(token_start);
        }
	}
	size_t start = locate.find("PRIVMSG ");
	if (start != std::string::npos)
	{
        start += 8;
		size_t end = locate.find(' ', start);
		if (end != std::string::npos)
        {
            user = '\0';
            msg = '\0';
            user = locate.substr(start, end - start); //destinataire
            msg = locate.substr(end + 2); // +2 pour ignorer ": "
            if (user[0] == '#')
 			{
 				user = user.substr(1);
 				is_channel = 1;
 			}
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
	for (std::map<std::string, Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
	{
		std::string channel_name = it->first;
		Channel *channel = it->second;
		if (channel_name == user)
		{
			std::map<int, Client *> client_list = channel->getClientlst();
			for (std::map<int,
					Client *>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
			{
				std::cout << YELLOW << "client dans channel = " << it_clt->second->getNickName() << RESET << std::endl;
			}
		}
	}
    std::string s_user = _clients[socket]->getUserName();
    std::string s_nick = _clients[socket]->getNickName();
	if (is_channel == 0)
 	{
		std::string msg_to_send = PRIVMSG(s_nick, user, msg);
		replyClient(msg_to_send, socket_to_send_to);
		(void) socket;
	}
	else
	{
		send_in_channel(user, s_nick, msg, socket, "send");
	}
}