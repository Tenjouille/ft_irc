#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Server.hpp"
#include "Client.hpp"

// ## MACROS ## //

// COMMANDS //
#define WELCOME_MSG(servername, username, nickname) (":" + servername + " 001 " + username + \
	" :Welcome to the Internet Relay Network " + username + "!" + nickname + "\r\n")

#define CAP_LS "CAP_ACK LS\r\n"

// ERRORS //
#define ERROR_INVPASS(username) (username + " :Password incorrect\r\n")

// OTHER

size_t	replyClient(std::string Macros, int socket);

// ## FUNCTIONS ## //
// size_t replyClient(std::string MACROS, int socket);


#endif