#ifndef SERVER_HPP
#define SERVER_HPP

#define RESET   "\x1b[0m"
#define BLACK   "\x1b[30m"
#define RED	 "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE	"\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN	"\x1b[36m"
#define WHITE   "\x1b[37m"

#include <sys/types.h> 
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include "Client.hpp"
#include "Communication.hpp"
#include "Channel.hpp"
//mode = conteneur set
class Client;
class Channel;

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define MAGENTA "\033[1;35m"
#define RED "\033[1;31m"
#define UNDER "\033[4m"

class Server
{
	private:
		int						_socket;
		struct sockaddr_in		_serverAddress;
		int					 	_clientSocket;
		std::string				_serverName;
		//liste de clients existant
		std::map<int, Client*>	_clients;
		std::string				_password;
		int						_status;
		fd_set					_allSockets;
		fd_set					_readFds;
		int						_fdMax;
		//liste de channels existant
		std::map<std::string, Channel*> _channelLst;
		int								_nb_channels;

	public:
		Server(char **arguments);
		void				closeSockets();
		//////////////GETTERS///////////////
		int					getSocket() const;
		int					getClientSocket(int socket) const;
		Client*				getClient(int socket) const; //! A REMETTRE CONST !!!!!//
		struct sockaddr_in  getServerAddress() const;
		// struct sockaddr	 getClientAddr() const;
		std::string			getServerPassword() const;
		std::string			getUsernameFormNick(std::string to_parse);
		int 				getSocketFromUser(std::string to_find);
		//SELECT()
		fd_set&				getallSockets();
		fd_set&				getreadFds();
		int					getfdMax();
		//////////////SETTERS///////////////
		void				setClientSocket(int tmp);
		void				setfdMax(int socket);
		///////////////LOOP/////////////////
		void				loop();
		void				read_data_from_socket(int socket);
		void				accept_new_connection();
		//////////////PARSING///////////////
		void				parser(std::string buffer, int socket);
		void				defineCmd(std::string cmd, int start, int it, int socket);
		std::string			defineArgs(std::string cmd);
		std::string			defineOptions(std::string cmd);
		//////////////COMMANDS//////////////
		void				caplsCmd(std::string locate, int socket);
		void				userCmd(std::string str, int socket);
		void				modeCmd(std::string locate, int socket);
		void				nickCmd(std::string str, int socket);
		void				passCmd(std::string to_parse, std::string cmd, int socket);
		void				joinCmd(std::string locate, int socket);
		void				quitCmd(int socket);
		void				pingCmd(std::string cmd, int socket);
		void 				msgCmd(std::string locate, int socket);
		ssize_t				sendToClient(std::string to_send, int socket);
		void				createChannel(std::string name, int socket);
		void    			send_in_channel(std::string user, std::string s_nick, std::string msg_to_send, int socket, std::string macro);
		bool				checkNickName(std::string to_check, int socket);
		void				topicCmd(std::string locate, int socket);
		void				delClient(int socket);

		void				inviteCmd(std::string locate, int socket);
		bool				fillinBuffer(std::string locate, std::string& servername, std::string& invited, std::string& nickname, int socket);
		void    			kickCmd(std::string locate, int socket);
		int				is_in_channel(std::string str, std::string channel);

		std::map<std::string, Channel*>::iterator findChannel(std::string channelName);
		int     isUser(std::string str);
		~Server();

		//check that the user is part of the channel before sending msg in channel
		bool    isPartOfChannel(int socket, std::map<int, Client*> client_list);
		bool    isInsideChannel(Channel *channel, int socket);
		bool   isClientOp(std::map<int, Client*> op_list, int socket);

		std::map<std::string, Channel*> getChannelListe() const;
		Channel *getChannelFromName(std::string name);
};

#endif