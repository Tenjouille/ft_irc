#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac == 3)
	{
		Server server(av);
		try
		{
			int status;
		   // struct timeval timer;
			status = listen(server.getSocket(), 10);
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