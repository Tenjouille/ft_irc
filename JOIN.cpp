#include "Server.hpp"

void Server::joinCmd(std::string locate, int socket)
{
        std::cout << RED << "locate = " << locate << std::endl;
        int flag = 0;
        size_t start = 0;
        start = locate.find("#");
        std::string channelName = locate.substr(start + 1);
        bool em = channelName.empty();
        std::cout << "start = " << start << "isempty = " << em << std::endl;
        if (start == std::string::npos || channelName.empty())
        {
            replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "JOIN"), socket);
            //std::cout << ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "JOIN") << std::endl;
            return;
        }
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
                {
                    std::cout << "ici topic = " << findChannel(channelName)->second->getTopic() << std::endl;
                    std::string tn = findChannel(channelName)->second->getTopic();
                    //replyClient(ALREADYTOPIC(getClient(socket)->getNickName(), channelName, findChannel(channelName)->second->getTopic()), socket);
                    std::cout << "tn = " << tn << std::endl;
                    replyClient(ALREADYTOPIC(getClient(socket)->getNickName(), channelName, tn), socket);
                }
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
        //combien de channel max peut recevoir un client?
        //set les key pour envoyer la bonne erreur si key incorrect
        //si on est ban
        //si client.limit
        //si le client est banni du channel
        //si le channel est sur invite uniquement
        //topic avec la valeur 333
        //353
}

void Server::createChannel(std::string name, int socket)
{
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