#include "Server.hpp"

// int Server::getSocketFromUser(std::string to_find)
// {
// 	int	socket;

// 	socket = -666;
// 	std::map<int, Client *>::iterator it = _clients.begin();
// 	if (it != _clients.end())
// 	{
// 		while (it != _clients.end())
// 		{
// 			if (it->second->getNickName() == to_find)
// 			{
// 				socket = it->first;
// 				//  std::cout << "Socket to send to : " << socket << std::endl;
// 				return (socket);
// 			}
// 			++it;
// 		}
// 		if (socket == -666)
// 			std::cout << RED << "ERROR SOCKET NOT FOUND" << RESET << std::endl;
// 	}
// 	else
// 	{
// 		std::cerr << RED << "ERROR D'ITERATOR" << RESET << std::endl;
// 	}
// 	return (socket);
// }

// void Server::msgCmd(std::string locate, int socket)
// {
// 	int		is_channel;
// 	size_t	start;
// 	size_t	end;
// 	int		socket_to_send_to;

// 	std::string user;
// 	std::string msg;
// 	// std::cout << RED << "locate = " << locate << RESET << std::endl;
// 	// std::cout << "On est ici : '" << locate << "'" << std::endl;
// 	is_channel = 0;
// 	// size_t ping_pos = locate.find("PING");
// 	// if (ping_pos != std::string::npos)
// 	// {
// 	//     std::string dest;
// 	//     std::string token;
// 	//     size_t privmsg_pos = locate.find("PRIVMSG");
// 	//     if (privmsg_pos != std::string::npos)
// 	//     {
// 	//         size_t dest_start = privmsg_pos + 8;
// 	//         size_t dest_end = locate.find(" ", dest_start);
// 	//         if (dest_end != std::string::npos)
// 	//         {
// 	//             dest = locate.substr(dest_start, dest_end - dest_start);
// 	//         }
// 	//         else
// 	//         {
// 	//             dest = locate.substr(dest_start);
// 	//         }
// 	//         std::cout << GREEN << dest << RESET << std::endl;       
// 		// void            setName();::endl;
// 	// }
// 	// size_t token_start = ping_pos + 5;
// 	// size_t token_end = locate.find("\r", token_start);
// 	// if (token_end != std::string::npos)
// 	// {
// 	//     token = locate.substr(token_start, token_end - token_start);
// 	// }
// 	// else
// 	// {
// 	//     token = locate.substr(token_start);
// 	// }
// 	// if (dest.empty())
// 	// {
// 	//     dest = "localhost";
// 	// }
// 	// std::string pong_message = PONG(dest, token);
// 	// // std::cout << "Sending : '" << pong_message << "'" << std::endl;
// 	// replyClient(pong_message, socket);
// 	// return ;
// 	// }
// 	// std::cout << "on est revenus ici ?" << std::endl;
// 	start = locate.find("PRIVMSG ");
// 	// std::cout << GREEN << "start = " << start << std::endl;
// 	if (start != std::string::npos)
// 	{
// 		start += 8;
// 		end = locate.find(' ', start);
// 		if (end != std::string::npos)
// 		{
// 			user = '\0';
// 			msg = '\0';
// 			user = locate.substr(start, end - start); //destinataire
// 			if (user[0] == '#')
// 			{
// 				user = user.substr(1);
// 				is_channel = 1;
// 			}
// 			msg = locate.substr(end + 2); // +2 pour ignorer ": "
// 			// user[user.length()] = '\0';
// 			// msg[msg.length()] = '\0';
// 			// std::cout << MAGENTA "User: '" << user << "'" << std::endl;
// 			// std::cout << MAGENTA "Message: '" << msg << "'" << std::endl;
// 		}
// 		else
// 		{
// 			std::cerr << "Erreur: Aucun espace trouvé après le nom d'utilisateur" << std::endl;
// 		}
// 	}
// 	else
// 	{
// 		std::cerr << "Erreur: Chaîne ne contient pas 'PRIVMSG '" << std::endl;
// 	}
// 	socket_to_send_to = getSocketFromUser(user);
// 	// int socket_to_send_to = _clients
// 	// std::map<int,Client*>::iterator it = _clients.find(socket);
// 	// std::string sender = it->second->getNickName();
// 	// std::string servername = "localhost";
// 	// std::string msg_to_send = MSG_TO_SEND(user, servername, sender, msg);
// 	// std::cout << MSG_TO_SEND(user, servername, sender, msg) << std::endl;
// 	std::string s_user = _clients[socket]->getUserName();
// 	std::string s_nick = _clients[socket]->getNickName();
// 	std::string msg_to_send = PRIVMSG(s_nick, user, msg);
// 	if (is_channel == 0)
// 	{
// 		std::cout << "ici" << std::endl;
// 		std::cout << "Username : " << s_user << "\nNickname : " << s_nick << "\nEnvoyer : " << user << "\nMsg : '" << msg << "'" << std::endl;
// 		// std::cout << "Sending : '" << msg_to_send << "'" << std::endl;
// 		// std::cout << socket_to_send_to << " et " << socket << std::endl;
// 		replyClient(msg_to_send, socket_to_send_to);
// 		(void)socket;
// 	}
// 	else
// 	{
//         std::cout << "Username : " << s_user << "\nNickname : " << s_nick << "\nEnvoyer : " << user << "\nMsg : '" << msg << "'" << std::endl;
// 		send_in_channel(user, s_nick, s_user, msg);
// 	}
// }
/*
void Server::send_in_channel(std::string user, std::string s_nick,
		std::string msg_to_send, int socket, std::string macro)
{
    //std::string msg = NULL;
//	std::string final_user = "#" + user;
    std::string final_user = user;
	//std::string msg = PRIVMSG(s_nick, final_user, msg_to_send);
    // msg = msg.substr(1);
   // std::cout << YELLOW << msg << RESET << std::endl;
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
			//	std::cout << "CLIENTS DU CHANNEL : " << it_clt->second->getNickName() << std::endl;
				if (it_clt->first != socket)
                {
					if (macro == "send")
                    {    
                        std::string msg = SENDINCHANNEL(s_nick, it_clt->second->getNickName(), msg_to_send, user);
					    replyClient(msg, it_clt->first);
                    }
                }
                else if (macro == "topic")
                {
                    std::string msg1 = TOPIC(it_clt->second->getNickName(), user, msg_to_send);
                    replyClient(msg1, it_clt->first);
                }
                    //     msg = TOPIC(it_clt->second->getNickName(), user, msg_to_send);
                    //std::cout << YELLOW << "socket = " << it_clt->first << std::endl;
			}
		}
	}
}*/

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

void Server::msgCmd(std::string locate, int socket)
{
	int is_channel = 0;
    std::string user;
    std::string msg;
	// std::cout << RED << "locate = " << locate << RESET << std::endl;    
    // std::cout << "On est ici : '" << locate << "'" << std::endl; 
	//string = "PRIVMSG uaupetit :salut ca va";
    
    size_t ping_pos = locate.find("PING");
    if (ping_pos != std::string::npos)
    {
        std::string dest;
        std::string token;

        // Trouver le destinataire
        size_t privmsg_pos = locate.find("PRIVMSG");
        if (privmsg_pos != std::string::npos)
        {
            size_t dest_start = privmsg_pos + 8; // Pour sauter "PRIVMSG "
            size_t dest_end = locate.find(" ", dest_start);
            if (dest_end != std::string::npos)
            {
                dest = locate.substr(dest_start, dest_end - dest_start);
            }
            else 
            {
                dest = locate.substr(dest_start);
            }
            std::cout << GREEN << dest << RESET << std::endl;
    }

    // Trouver le token
    size_t token_start = ping_pos + 5; // Pour sauter "PING "
    size_t token_end = locate.find("\r", token_start);
    if (token_end != std::string::npos)
    {
        token = locate.substr(token_start, token_end - token_start);
    }
    else
    {
        token = locate.substr(token_start);
    }
    if (dest.empty())
    {
        dest = "localhost";
    }
    
    // std::string pong_message = PONG(dest, token);
    // std::cout << "Sending : '" << pong_message << "'" << std::endl;
    // replyClient(pong_message, socket);
    return;
	}

 //   std::cout << "on est revenus ici ?" << std::endl;
	size_t start = locate.find("PRIVMSG ");
	//std::cout << GREEN << "start = " << start << std::endl;
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
			// user[user.length()] = '\0';
            // msg[msg.length()] = '\0';
          //  std::cout << MAGENTA "User: '" << user << "'" << std::endl;
            //std::cout << MAGENTA "Message: '" << msg << "'" << std::endl;
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
    // int socket_to_send_to = _clients
    
	// std::map<int,Client*>::iterator it = _clients.find(socket);

    // std::string sender = it->second->getNickName(); 
    // std::string servername = "localhost";
    // std::string msg_to_send = MSG_TO_SEND(user, servername, sender, msg);
    // std::cout << MSG_TO_SEND(user, servername, sender, msg) << std::endl;
	/**********************printt client   */
	//std::cout << YELLOW << "channel name = " << user << RESET << std::endl;
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
		// std::cout << RED << "LALALALAAAA" << RESET << std::endl;
		// std::cout << "Username : " << s_user << "\nNickname : " << s_nick << "\nEnvoyer : " << user << "\nMsg : '" << msg << "'" << std::endl;
		std::string msg_to_send = PRIVMSG(s_nick, user, msg);
		//std::cout << "Sending : '" << msg_to_send << "'" << std::endl; 
		//std::cout << socket_to_send_to << " et " << socket << std::endl;
		// std::string servername = "localhost";
		//std::cout << YELLOW << "SOCKET DU SERVER : " << _socket << RESET << std::endl;
		// replyClient(DEFAULTPONG(s_nick), socket_to_send_to);
		replyClient(msg_to_send, socket_to_send_to);
		(void) socket;
	}
	else
	{
	//	std::cout << RED << "ICICICIIIIC" << RESET << std::endl;
	//	std::cout << "Username : " << s_user << "\nNickname : " << s_nick << "\nEnvoyer : " << user << "\nMsg : '" << msg << "'" << std::endl;
		send_in_channel(user, s_nick, msg, socket, "send");
	}
}