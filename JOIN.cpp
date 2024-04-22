#include "Server.hpp"

void Server::joinCmd(std::string locate, int socket)
{
    // size_t start = locate.find("#");
    // if (start != std::string::npos)
    // {
    //     size_t end = locate.find(" ", start);
    //     if (end != std::string::npos)
    //     {

        // std::string channel_name;
        // size_t i = 5; //skip JOIN #
        // while (locate[i] != '\0')
        // {
        //     channel_name += locate[i];
        //     i++;
        // }
        // channel_name += '\0';
        size_t start = locate.find("#");
        std::string channelName = locate.substr(start + 1);
        std::cout << "CHANNEL NAME MIEUX : " << channelName << std::endl;
        // std::string channelName = locate.substr(start + 1, end - start - 1);
        for (std::map<std::string,
		Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
	    {
            std::cout << "iteration" << std::endl;
		    std::string tmp = it->first;
		    Channel *channel = it->second;
		    if (tmp == channelName)
            {
                std::cout << "added one client" << std::endl;
                channel->addClient(socket, getClient(socket));
                return; 
            }
            // getClient(socket)->getChannel().push_back(name);
            // replyClient(CREATECHANNEL(getClient(socket)->getName(), getClient(socket)->getUserName(), name), socket)
        }
        // else
        // {
            std::cout << "Nom du canal : " << channelName << std::endl;
            createChannel(channelName, socket);
    //    }
       

    ///ATTENTON CHECK SI # A UN MOMENT
    // else
	// {
    //     std::cout << "Aucun symbole '#' trouvé dans la chaîne." << std::endl;
    // }
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
    std::cout << "here i am" << std::endl;
    if (_channelLst.find(name) == _channelLst.end())
    {
        replyClient(NOT_EXISTING_CHANNEL(name), socket);
    }
    Channel* channel = new Channel(name);
    _channelLst.insert(std::make_pair(name, channel));
    std::cout << "CHANNEL ADDED" << std::endl;
    channel->newOperator(socket, getClient(socket));
    channel->addClient(socket, getClient(socket));
    getClient(socket)->getChannel().push_back(name);
    replyClient(CREATECHANNEL(getClient(socket)->getName(), getClient(socket)->getUserName(), name), socket);
}

    //verifier si le channel existe deja 
	//creer channel
	//y ajouter le client