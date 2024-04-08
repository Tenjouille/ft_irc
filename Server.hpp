#ifndef SERVER_HPP
#define SERVER_HPP

#define RESET   "\x1b[0m"
#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
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

class Client;

class Server
{
	private:
		int _socket;
		struct sockaddr_in	  _serverAddress;
		int					 _clientSocket;
		std::map<int, Client*>  _clients;
		std::string			 _password;
		int 				_status;
		fd_set _allSockets;
		fd_set _readFds;
		int _fdMax;

	public:
		Server(char **arguments);
		void				parser(char *buffer, int socket);
		void				loop();
		void				read_data_from_socket(int socket);
		void				accept_new_connection();
	
		//GETTERS
		int				getSocket() const;
		int				getClientSocket(int socket) const;
		Client*			getClient(int socket) const;
		struct sockaddr_in  getServerAddress() const;
		// struct sockaddr	 getClientAddr() const;
		std::string getServerPassword() const;

		//SETTERS
		void				setClientSocket(int tmp);


	  	//CMD
	    void                caplsCmd(std::string locate, int socket);
		void				defineCmd(std::string cmd, int start, int it, int socket);
		void				userCmd(std::string str, int socket);
		void				nickCmd(std::string str, int socket);
		void	passCmd(std::string cmd, int socket);
		void 	joinCmd(std::string locate, int socket);
		ssize_t sendToClient(std::string to_send, int socket);

		void	setfdMax(int socket);
		fd_set& getallSockets();
		fd_set& getreadFds();
		int getfdMax();
		void	quitCmd(int socket);
		void	delClient(int socket);

        ~Server();

};

#endif