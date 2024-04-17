#include "Server.hpp"

void	Server::pingCmd(std::string cmd, int socket)
{
	std::cout << "CELUI LA" << std::endl;
	(void) cmd;
	std::string servername = "localhost";
	sleep(1);
    replyClient(DEFAULTPONG(servername), socket);
}

// void	Server::pingCmd(std::string locate, int socket)
// {
    
//     size_t privmsg_pos = locate.find("PRIVMSG");
    
// 	//PING LOCALHOST
// 	if (privmsg_pos == std::string::npos)
// 	{
// 		std::cout << "ICIIIII ????????" << std::endl;
// 		std::string send_to;
// 		int i = 5; //skip le "PING"
// 		while (locate[i] != '\0')
// 		{
// 			send_to += locate[i]; 
// 			i++;
// 		}
// 		send_to += '\0';
// 		// std::cout << "Sending : '" << PONG_NOTOKEN(send_to) << "'" << std::endl;
// 		// std::string pong_msg = PONG_NOTOKEN(send_to);
// 		std::string servername = "localhost";
//         replyClient(DEFAULTPONG(servername), socket);
// 		// replyClient(pong_msg, socket);
// 		return ;
// 	}
//     else //PING WITH TOKEN
// 	{
// 		size_t ping_pos;
// 		ping_pos = locate.find("PING");
//        std::string dest;
//        std::string token;
//        // Trouver le destinataire
//        if (privmsg_pos != std::string::npos)
//        {
//            size_t dest_start = privmsg_pos + 8; // Pour sauter "PRIVMSG "
//            size_t dest_end = locate.find(" ", dest_start);
//            if (dest_end != std::string::npos)
//            {
//                dest = locate.substr(dest_start, dest_end - dest_start);
//            }
//            else 
//            {
//                dest = locate.substr(dest_start);
//            }
//            std::cout << GREEN << dest << RESET << std::endl;
//    	}

//     // Trouver le token
//     size_t token_start = ping_pos + 5; // Pour sauter "PING "
//     size_t token_end = locate.find("\r", token_start);
//     if (token_end != std::string::npos)
//     {
//         token = locate.substr(token_start, token_end - token_start);
//     }
//     else
//     {
//         token = locate.substr(token_start);
//     }

//     if (dest.empty())
//     {
//         dest = "localhost";
//     }
    
//     std::string pong_message = PONG(dest, token);
//     std::cout << "Sending : '" << pong_message << "'" << std::endl;
// 	std::string servername = "localhost";
//     replyClient(DEFAULTPONG(servername), socket);
//     return;
// }
