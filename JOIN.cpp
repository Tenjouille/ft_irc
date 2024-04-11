#include "Server.hpp"

void Server::joinCmd(std::string locate, int socket)
{
    (void)socket;
    // string = join #channel
    size_t start = locate.find("#");
    if (start != std::string::npos) { // Vérifie si le symbole '#' est trouvé
        size_t end = locate.find(" ", start); // Trouve le prochain espace après le symbole '#'
        if (end != std::string::npos) { // Vérifie si un espace est trouvé
            std::string channelName = locate.substr(start + 1, end - start - 1); // Extrait le nom du canal
            std::cout << "Nom du canal : " << channelName << std::endl;
        } else { // Si aucun espace n'est trouvé après le symbole '#', le reste de la chaîne est le nom du canal
            std::string channelName = locate.substr(start + 1);
            std::cout << "Nom du canal : " << channelName << std::endl;
        }
    } else
	{
        std::cout << "Aucun symbole '#' trouvé dans la chaîne." << std::endl;
    }
	//creer channel
	//y ajouter le client
}

void Server::createChannel(std::string name, int socket)
{
	Channel* channel = new Channel(name);
	_channelLst.insert(std::make_pair(name, channel));
	channel->addClient(socket, getClient(socket));
	getClient(socket)->getChannel().push_back(name);
	replyClient(CREATECHANNEL(getClient(socket)->getName(), getClient(socket)->getUserName(), name), socket);
}