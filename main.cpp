#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac == 3)
	{
		try
		{
			int status;
			Server server(av);
		   // struct timeval timer;
			status = listen(server.getSocket(), 10);
			// Préparation des ensembles de sockets pour select()
			FD_ZERO(&server.getallSockets());
			FD_ZERO(&server.getreadFds());
			FD_SET(server.getSocket(), &server.getallSockets()); // Ajout de la socket principale à l'ensemble
			server.setfdMax(server.getSocket()); // Le descripteur le plus grand est forcément celui de notre seule socket
			server.loop();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else
		std::cout << "Bad arguments number" << std::endl;
}