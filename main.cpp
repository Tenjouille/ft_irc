#include "Server.hpp"

bool	interrupted = false;

void handleSignal(int signal_recu)
{
	if (signal_recu == SIGINT)
	{
		std::cout << "Closing SERVEUR" << std::endl;
		std::cout << "Closing SERVEUR" << std::endl;
		std::cout << "Closing SERVEUR" << std::endl;
		std::cout << RESET << RED << "Closing SERVEUR" << RESET << std::endl;
		interrupted = true;
		return ;
	}
	return ;
}

void	Server::loop()
{
	struct timeval timer;
	// int status; commented because error set but not used
	signal(SIGINT, handleSignal);
	while (!interrupted)
	{
		_readFds = _allSockets;
		timer.tv_sec = 2;
		timer.tv_usec = 0;
		int result;
		result = select(_fdMax + 1, &_readFds, NULL, NULL, &timer);
		// signal(SIGINT, handleSignal);
		if (result == -1 || interrupted == true)
		{
			std::cout << "INTERUP : " << interrupted << std::endl;
			std::cout << "ERROR WITH SELECT" << std::endl;	
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
		std::cout << "Bad arguments number" << std::endl;
}