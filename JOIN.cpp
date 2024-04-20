#include "Server.hpp"

    ///ATTENTON CHECK SI # A UN MOMENT
    // else
	// {
    //     std::cout << "Aucun symbole '#' trouvé dans la chaîne." << std::endl;
    // }

void Server::joinCmd(std::string locate, int socket)
{
        std::cout << RED << "locate = " << locate << std::endl;
        int flag = 0;
        size_t start = locate.find("#");
        std::string channelName = locate.substr(start + 1);
        for (std::map<std::string,
		Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
	    {
		    std::string tmp = it->first;
		    Channel *channel = it->second;
		    if (tmp == channelName)
            {
                flag = 1;
                channel->addClient(socket, getClient(socket));
                if (findChannel(channelName)->second->getTopicStatus() == 1)
                    replyClient(ALREADYTOPIC(getClient(socket)->getNickName(), channelName, findChannel(channelName)->second->getTopic()), socket);
                else
                    replyClient(NOTOPIC(getClient(socket)->getNickName(), channelName), socket);
                break; 
            }
        }
        if (flag == 1)
        {
            std::map<std::string, Channel*>::iterator channelIt = findChannel(channelName);
            if (channelIt != _channelLst.end())
            {
                std::map<int, Client*> clientMap = channelIt->second->getClientlst();
                std::string userlst;
                for (std::map<int, Client*>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
                {
                    userlst += it->second->getNickName() + " ";
                }
                replyClient(LISTUSERS(getClient(socket)->getNickName(), channelName, userlst), socket);
                replyClient(NAMELIST(getClient(socket)->getNickName(), channelName), socket);
            }
        }
        else if (flag == 0)
            createChannel(channelName, socket);
        //il faudra envoyer un reply avec la liste des clients du channel dans tout les cas
        
        // std::string userlst;
}

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
    if (findChannel(name)->second->getTopicStatus() == 0)
        replyClient(NOTOPIC(getClient(socket)->getNickName(), name), socket);
    std::cout << "imprime ca :'" << getClient(socket)->getName() << "'" << std::endl;
    replyClient(LISTUSERS(getClient(socket)->getNickName(), name, getClient(socket)->getNickName()), socket);
    replyClient(NAMELIST(getClient(socket)->getNickName(), name), socket);
}

std::map<std::string, Channel*>::iterator Server::findChannel(std::string channelName)
{
    for(std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if (it->second->getName() == channelName)
            return it;
    }
    return _channelLst.end();
}