#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Server.hpp"
#include "Client.hpp"

// COMMANDS //
#define WELCOME_MSG(servername, username, nickname) (":" + servername + " 001 " + username + \
	" :Welcome to the Internet Relay Network " + username + "!" + nickname + "\r\n")
#define CAP_LS "CAP_ACK LS\r\n"
//#define PONffG(servername, token) (":" + token + " PONG " + servername + "\r\n")
//#define PONG(servername) ("PONG " + servername + "\r\n")
//#define PRIVMSG(nickname, envoyeur, msg) (":" + nickname + "!" + "@localhost" + " PRIVMSG " + nickname + " :" + msg + "\r\n")
#define PRIVMSG(nickname, envoyeur, msg) (":" + nickname + "!" + "localhost" + " PRIVMSG " + envoyeur + " :" + msg + "\r\n")
#define TOPIC(nickname, channelName, topic) (":" + nickname + "!" + "uaupetit@localhost" + " TOPIC #" + channelName + " :" + topic + "\r\n") 
#define CREATECHANNEL(nickname, username, name) (":" + nickname+ "!" + username + "@localhost JOIN #" + name + "\r\n")
#define NOT_EXISTING_CHANNEL(channelName) (":localhost 403 #" + channelName + " :No such channel\r\n")
#define SENDINCHANNEL(sender, receiver, msg, channelname) (":" + sender + "!" + "uaupetit" + "@localhost PRIVMSG #" + channelname + " :" + msg + "\r\n")
// ERRORS //
// #define ERROR_INVPASS(username) (username + " :Password incorrect\r\n")
#define NICKNAMEINUSE_ERR(username) (":localhost 433 * " + username + " :Nickname is already in use.\r\n")
// #define PRIVMSG(argument_to_send, servername) ()
// #define PONG(servername, token) (":PONG " + token + " " + servername + "\r\n")
//#define PONG(servername, token) (":" + token + "PONG " + servername + "\r\n")
#define DEFAULTPONG(servername) ("PONG " + servername + "\r\n")

#define PRIVMSG(nickname, envoyeur, msg) (":" + nickname + "!" + "localhost" + " PRIVMSG " + envoyeur + " :" + msg + "\r\n")

// ERRORS //
#define ERROR_INVPASS(nickname) (nickname + " :Password incorrect\r\n")
// #define NICKNAMEINUSE_ERR(username) (":localhost 433 * " + username + " :Nickname is already in use.")


#define NOTOPIC(nickname, channelName) (":localhost 331 " + nickname + " #" + channelName + " :No topic is set\r\n")
#define ALREADYTOPIC(nickname, channelName, topicName) (":localhost 332 " + nickname + " #" + channelName + " " + topicName + "\r\n")
#define LISTUSERS(nickname, channelName, userslst) (":localhost 353 "+ nickname + " = #" + channelName+ " :@" + userslst + "\r\n")
#define NAMELIST(user, channel) (":localhost 366 " + user + " " + channel + " :End of /NAMES list\r\n")
// OTHER

size_t	replyClient(std::string Macros, int socket);
//#define PONG(servername, token) (":" + token + "PONG " + servername + "\r\n")
// draft
// ## FUNCTIONS ## //
// size_t replyClient(std::string MACROS, int socket);
// #define PRIVMSG(argument_to_send, servername) ()
// #define PONG(servername, token) (":PONG " + token + " " + servername + "\r\n")

#endif