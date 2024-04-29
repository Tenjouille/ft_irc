#include "Server.hpp"

bool    Server::isPartOfChannel(int socket, std::map<int, Client*> client_list)
{
    std::map<int, Client*>::iterator it = client_list.begin();
    std::map<int, Client*>::iterator ite = client_list.end();

    while (it != ite)
    {
        if (it->first == socket)
        {
            std::cout << "CLIENT FOUND : " << socket << " and " << it->first << std::endl;
            return (true);
        }
        ++it;
    }
    return (false);
}

void Server::send_in_channel(std::string user, std::string s_nick, std::string msg_to_send, int socket, std::string macro)
{
    std::string final_user = user;
    bool    channel_found = false;
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        std::string channel_name = it->first;
        Channel *channel = it->second;
        if (channel_name == user)
        {
            channel_found = true;
            std::map<int, Client*> client_list = channel->getClientlst();
            if (macro == "send")
            {
                if (isPartOfChannel(socket, client_list) == false)
                {
                    // THE USER IS NOT PART OF THE CHANNEL
                    std::cout << "SOCKET WASNT FOUND IN CLIENT LIST\n" << std::endl;
                    std::string err_msg = ERR_NOTONCHANNEL(_clients[socket]->getNickName(), user);
                    std::cout << RED << "SENDING : " << err_msg << RESET << std::endl;
                    replyClient(err_msg, socket);
                    return ; 
                }
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
    if (channel_found == false)
    {
        std::string err_msg = ERR_NOSUCHCHANNEL(user);
        replyClient(err_msg, socket);
        return ;
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
        {
            if (to_find == "Botimus_Maximus")
            {
                socket = -777;
                std::cout << YELLOW << "TALKING TO THE BOT" << RESET << std::endl;
                return (socket);
            }
            else
                std::cout << RED << "ERROR SOCKET NOT FOUND" << RESET << std::endl;
        }
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

bool Server::msgCheckArgs(std::string locate)
{
    size_t found;
    found = locate.find(":", 0);
    std::cout << "found at : " << found << std::endl;
    if (found == std::string::npos)
        return (false);
    return (true);
}

void Server::msgCmd(std::string locate, int socket)
{
    std::cout << "LOCATE : " << locate << std::endl;
    if (msgCheckArgs(locate) == false)
    {
        replyClient(ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "PRIVMSG"), socket);
        return ;
    }
	int is_channel = 0;
    std::string user;
    std::string msg;
	size_t start = locate.find("PRIVMSG ");
	if (start != std::string::npos)
	{
        start += 8;
		size_t end = locate.find(' ', start);
		if (end != std::string::npos)
        {
            size_t test = locate.find(':', start);
            user = '\0';
            msg = '\0';
            user = locate.substr(start, end - start); //destinataire
            if (locate[test] == ':')
                msg = locate.substr(test + 1); // +2 pour ignorer ": "
            else
                msg = locate.substr(test);
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
	for (std::map<std::string,
			Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
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
        int socket_to_send_to = getSocketFromUser(user);
        
        //TALKING TO THE BOT
        if (socket_to_send_to == -777)
        {
            //parsing bot
            parsingBot(msg, user, socket);
            return ;
        }
        
        //USER TO SEND TO NOT FOUND
        if (socket_to_send_to == -666)
        {
            std::cout << "ERROR : TARGET NOT FOUND !" << std::endl;
            std::string no_such_nick = ERR_NOSUCHNICK(s_nick, user);
            std::cout << "Sending error message : " << no_such_nick << std::endl;
            replyClient(no_such_nick, socket);
            return ;
        }

		std::cout << "Username : " << s_user << "\nNickname : " << s_nick << "\nEnvoyer : " << user << "\nMsg : '" << msg << "'" << std::endl;

		std::string msg_to_send = PRIVMSG(s_nick, user, msg);
		std::cout << "Sending : '" << msg_to_send << "'" << std::endl; 

		std::cout << socket_to_send_to << " et " << socket << std::endl;
		replyClient(msg_to_send, socket_to_send_to);
		(void) socket;
	}
	else
	{
		send_in_channel(user, s_nick, msg, socket, "send");
	}
}