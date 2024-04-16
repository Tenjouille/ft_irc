#include "Server.hpp"

void Server::joinCmd(std::string locate, int socket)
{
    size_t start = locate.find("#");
    if (start != std::string::npos)
    {
        size_t end = locate.find(" ", start);
        if (end != std::string::npos)
        {
            std::string channelName = locate.substr(start + 1, end - start - 1);
            std::cout << "Nom du canal : " << channelName << std::endl;
        }
        else
        {
            std::string channelName = locate.substr(start + 1);
            std::cout << "Nom du canal : " << channelName << std::endl;
            createChannel(channelName, socket);
        }
    }
    else
	{
        std::cout << "Aucun symbole '#' trouvé dans la chaîne." << std::endl;
    }
    
}

/*void Server::createChannel(std::string name, int socket)
{
	Channel* channel = new Channel(name);
	_channelLst.insert(std::make_pair(name, channel));
	channel->addClient(socket, getClient(socket));
	getClient(socket)->getChannel().push_back(name);
	replyClient(CREATECHANNEL(getClient(socket)->getName(), getClient(socket)->getUserName(), name), socket);
}*/

void Server::createChannel(std::string name, int socket)
{
    if (_channelLst.find(name) == _channelLst.end())
    {
        replyClient(NOT_EXISTING_CHANNEL(name), socket);
    }
    Channel* channel = new Channel(name);
    _channelLst.insert(std::make_pair(name, channel));
    channel->addClient(socket, getClient(socket));
    getClient(socket)->getChannel().push_back(name);
    replyClient(CREATECHANNEL(getClient(socket)->getName(), getClient(socket)->getUserName(), name), socket);
}

    //verifier si le channel existe deja 
	//creer channel
	//y ajouter le client