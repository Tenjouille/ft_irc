#include "Server.hpp"


int main(int ac, char **av)
{
	int status;
	if (ac == 3)
	{
		Server server(av);
		try
		{
		   // struct timeval timer;
			status = listen(server.getSocket(), 10);
			(void) status;
			FD_ZERO(&server.getallSockets());
			FD_ZERO(&server.getreadFds());
			FD_SET(server.getSocket(), &server.getallSockets());
			server.setfdMax(server.getSocket());
			// signal(SIGINT, handleSignal);
			server.loop();
			// signal(SIGINT, handleSignal);
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