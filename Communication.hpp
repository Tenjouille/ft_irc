#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Server.hpp"
#include "Client.hpp"

// COMMANDS //

#define WELCOME_MSG(servername, username, nickname) (":" + servername + " 001 " + username + \
	" :Welcome to the Internet Relay Network " + username + "!" + nickname + "\r\n")
#define CAP_LS() ("CAP_ACK LS\r\n")
#define CREATECHANNEL(nickname, username, name) (":" + nickname+ "!" + username + "@localhost JOIN " + name + "\r\n") //CREATE CHANNEL
#define SENDINCHANNEL(sender, receiver, msg, channelname) (":" + sender + "!" + "uaupetit" + "@localhost PRIVMSG #" + channelname + " :" + msg + "\r\n") //PRIVMSG FOR CHANNEL
#define PRIVMSG(nickname, envoyeur, msg) (":" + nickname + "!" + "localhost" + " PRIVMSG " + envoyeur + " :" + msg + "\r\n") //PRIVMSG CMD
#define DEFAULTPONG(servername) ("PONG " + servername + "\r\n") //PONG CMD
#define RPL_INVITING(userID, client_nickname, invited_nickname, channel_name) (userID + " INVITE " + invited_nickname + " :" + channel_name + "\r\n") //INVITE CMD
//!#define NICK(userID, nickname) (userID + " NICK :" + nickname + "\r\n")

// NICK ERRORS //
#define NICKNAMEINUSE_ERR(username) (":localhost 433 * " + username + " :Nickname is already in use." + "\r\n")
// #define NICKNAMEINUSE_ERR(nickname) (":localhost 433 " + nickname + ":Nickname is already in use" + "\r\n") //NICKNAME ALREADY USED BY SOMEONE
#define ERR_ERRONEUSNICKNAME(client_username, nick) (client_username + " " + nick + " :Erroneus nickname" + "\r\n") //THERE IS A FORDBIDEN CHAR IN THE NICKNAME

// PRIVMSG ERRORS
#define ERR_NOSUCHNICK(client_nickname, target_nickname) (client_nickname + " " + target_nickname + " :No such nick/channel" + "\r\n")

// PASS ERRORS //
#define ERROR_INVPASS(nickname) (nickname + " :Password incorrect\r\n")

// INVITE ERRORS //
#define ERR_NOTONCHANNEL(client_nickname, channelname) (client_nickname + " " + channelname + ":You're not on that channel" + "\r\n") //TRYING TO INVITE SOMEONE INTO A CHANNEL YOUR ARE NOT PART OF
#define ERR_NOSUCHCHANNEL(channelName) (":localhost 403 " + channelName + " :No such channel" + "\r\n") //NEEDS A # IN THE CHANNEL NAME !!
#define ERR_USERONCHANNEL(client, nickname , channelname) (":localhost 403 " + client + " " + nickname + " " \
+ channelname + " :is already on channel" + "\r\t") //TRYING TO INVITE SOMEONE THAT IS ALREADY IN THE CHANNEL

// JOIN ERRORS //

#define ERR_CHANNELISFULL(client_nickname, channel_name) (client_nickname + " " + channel_name + " :Cannot join channel (+l)" + "\r\n") //TRYING TO JOIN A CHANNEL THAT IS FULL
#define NOT_EXISTING_CHANNEL(channelName) (":localhost 403 #" + channelName + " :No such channel\r\n") //TRYING TO JOIN A CHANNEL THAT DOESNT EXIST

#define userID(nickname, username) (":" + nickname + "!" + username + "@localhost")
// FUNCTION 

size_t	replyClient(std::string Macros, int socket);

#endif