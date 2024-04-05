#include "Communication.hpp"

size_t	replyClient(std::string Macros, int socket)
{
	size_t bytes_sent = 0;

	std::cout << "before" << std::endl;
	bytes_sent = send(socket, Macros.c_str(), Macros.size(), 0);
	if (bytes_sent == static_cast<size_t>(-1))
	{
		std::cout << "Something went wrong !" << std::endl;
		return (bytes_sent);
	}
	std::cout << "SENT : \"" << Macros << "\"" << std::endl;
	std::cout << "of size : " << static_cast<int>(bytes_sent) << " (" << bytes_sent << ")" <<std::endl;
	return (bytes_sent);
}


/* HOW TO USE */ /*
- Lancer le programme avec un port en argument
- Se connecter avec "nc localhost <port>"
- Si tout marche bien les messages apparaissent sur le client
*/
int	main(int ac, char **av)
{
	(void) ac;

	// VARIABLES
	std::string servername = "localhost";
	std::string username = "vgoret";
	std::string nickname = "le_v";

	struct sockaddr_in	serverAddress;
	int port = atoi(av[1]);
	int	_socket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(port);
    bind(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	
	// TESTING
	std::cout << WELCOME_MSG(servername, username, nickname) << std::endl;
	std::cout << ERROR_INVPASS(username) << std::endl;

	// Après la création du socket et le bind, ajoutez la ligne suivante pour configurer le socket à écouter les connexions entrantes
	listen(_socket, SOMAXCONN);

	// Accepter les connexions entrantes
	int clientSocket = accept(_socket, NULL, NULL);
	if (clientSocket < 0) {
	    std::cerr << "Error accepting connection" << std::endl;
	    close(_socket); // Fermer le socket serveur en cas d'erreur
	    return 1;
	}

	// Ensuite, vous pouvez envoyer les messages au client connecté
	replyClient(WELCOME_MSG(servername, username, nickname), clientSocket);
	replyClient(ERROR_INVPASS(username), clientSocket);

	// N'oubliez pas de fermer les sockets après avoir terminé
	close(clientSocket);
	close(_socket);

	return (0);
}