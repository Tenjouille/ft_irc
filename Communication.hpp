#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Server.hpp"
#include "Client.hpp"

// ## MACROS ## //

// COMMANDS //
#define WELCOME_MSG(servername, username, nickname) (":" + servername + " 001 " + username + \
	" :Welcome to the Internet Relay Network " + username + "!" + nickname + "\r\n")

#define CAP_LS "CAP_ACK LS\r\n"
// #define PRIVMSG(argument_to_send, servername) ()
// #define PONG(servername, token) (":PONG " + token + " " + servername + "\r\n")
#define PONG(servername, token) (":" + token + "PONG " + servername + "\r\n")

#define PRIVMSG(nickname, envoyeur, msg) (":" + nickname + "!" + "localhost" + " PRIVMSG " + envoyeur + " :" + msg + "\r\n")
#define CREATECHANNEL(nickname, username, name) (":" + nickname+ "!" + username + "@localhost JOIN " + name + "/r/n")
#define NOT_EXISTING_CHANNEL(channelName) (":localhost 403 #" + channelName + " :No such channel\r\n")
// ERRORS //
#define ERROR_INVPASS(username) (username + " :Password incorrect\r\n")
#define NICKNAMEINUSE_ERR(username) (":localhost 433 * " + username + " :Nickname is already in use.\r\n")


// OTHER

size_t	replyClient(std::string Macros, int socket);

// ## FUNCTIONS ## //
// size_t replyClient(std::string MACROS, int socket);


#endif