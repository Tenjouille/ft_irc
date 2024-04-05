#ifndef SERVER_HPP
#define SERVER_HPP
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

	public:
		Server(char **arguments);
		void				parser(char *buffer, int socket);
		void				loop(fd_set all_sockets, fd_set read_fds, int fd_max);
		void				read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket);
		void				accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max);
	
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

		ssize_t sendToClient(std::string to_send, int socket);

        ~Server();

};

#endif