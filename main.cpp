#include "Server.hpp"

bool	interrupted = false;

void handleSignal(int signal_recu)
{
	if (signal_recu == SIGINT)
	{
		interrupted = true;
		return ;
	}
	return ;
}

void	Server::loop()
{
	struct timeval timer;
	signal(SIGINT, handleSignal);
	while (!interrupted)
	{
		_readFds = _allSockets;
		timer.tv_sec = 2;
		timer.tv_usec = 0;
		int result;
		result = select(_fdMax + 1, &_readFds, NULL, NULL, &timer);
		if (result == -1)
		{
			std::cout << "Closing Server" << std::endl;
			return;
		}
		if (interrupted == true)
		{
			return ;
		}
		for (int i = 0; i <= _fdMax; i++)
		{
			if (FD_ISSET(i, &_readFds) != 1)
				continue ;
			if (i == _socket)
				accept_new_connection();
			else
				read_data_from_socket(i);
		}
	}
}

int main(int ac, char **av)
{
	int status;
	if (ac == 3)
	{
		Server server(av);
		try
		{
			status = listen(server.getSocket(), 10);
			(void) status;
			FD_ZERO(&server.getallSockets());
			FD_ZERO(&server.getreadFds());
			FD_SET(server.getSocket(), &server.getallSockets());
			server.setfdMax(server.getSocket());
			server.loop();
		}
		catch(const std::exception& e)
		{
			server.closeSockets();
			std::cerr << e.what() << '\n';
		}
	}
	else
		std::cout << "Usage : ./ircserv *port* *mdp*" << std::endl;
}