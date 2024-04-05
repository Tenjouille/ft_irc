#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac == 3)
	{
		try
		{
			int status;
			Server server(av);
			fd_set all_sockets; // Ensemble de toutes les sockets du serveur
			fd_set read_fds;	// Ensemble temporaire pour select()
			int fd_max;		 // Descripteur de la plus grande socket
		   // struct timeval timer;
			status = listen(server.getSocket(), 10);
			// Préparation des ensembles de sockets pour select()
			FD_ZERO(&all_sockets);
			FD_ZERO(&read_fds);
			FD_SET(server.getSocket(), &all_sockets); // Ajout de la socket principale à l'ensemble
			fd_max = server.getSocket(); // Le descripteur le plus grand est forcément celui de notre seule socket
			server.loop_bis(all_sockets, read_fds, fd_max);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else
		std::cout << "Bad arguments number" << std::endl;
}