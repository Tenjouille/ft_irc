#include "Server.hpp"

bool    Server::isInsideChannel(Channel *channel, int socket)
{
    std::map<int, Client*>client_list = channel->getClientlst();
    std::map<int, Client *>::iterator it = client_list.begin();
    std::map<int, Client *>::iterator ite = client_list.end();
    
    while (it != ite)
    {
        if (it->first == socket)
        {
            std::cout << GREEN << "Client's SOCKEt found !" << RESET << std::endl;
            return (true);
        }
        ++it;
    }

    return (false);
}

void extractStringAndFill(std::string& input, std::string& afterSpace)
{
    size_t pos = input.find(' ');
    if (pos != std::string::npos && pos + 1 < input.size()) // Vérifie si un espace est trouvé et qu'il y a du texte après
    {
        afterSpace = input.substr(pos + 1); // Remplit la chaîne après l'espace
        input.resize(pos); // Coupe la chaîne d'entrée au niveau de l'espace
    }
    // Si aucun espace n'est trouvé ou s'il se trouve à la fin de la chaîne, la chaîne d'entrée reste inchangée
}


void Server::joinCmd(std::string locate, int socket)
{
        std::cout << RED << "locate = " << locate << std::endl;
        int flag = 0;
        size_t start = locate.find("#");
        std::string channelName = locate.substr(start + 1);
        std::cout << "LEN : " << channelName.length() << " For : '" << channelName << "'" << std::endl;
        
        std::string key;
        //CUT STRING IF THERE IS A " " IN THE CHANNEL NAME AND ADDS INTO KEY
        if (channelName.find(" ") != std::string::npos)
        {
            extractStringAndFill(channelName, key);
            std::cout << "Channelname : " << channelName << std::endl;
            if (key != "")
                std::cout << "Key : " << key << std::endl;
        }

        std::cout << GREEN << "LEN : " << channelName.length() << " For : '" << channelName << "'" << RESET << std::endl;
        for (std::map<std::string,
		Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
	    {
		    std::string tmp = it->first;
		    Channel *channel = it->second;
		    if (tmp == channelName)
            {
                // INVITE ONLY && CLIENT IS NOT INSIDE OR IS NOT INVITED
                if (channel->getInvitOnly() == true && isInsideChannel(channel, socket) == false)
                {
                    std::cout << channel->getInvitOnly() << " and : " << isInsideChannel(channel, socket) << std::endl;
                    std::string err_msg = ERR_INVITEONLYCHAN(_clients[socket]->getNickName(), channelName);
                    std::cout << "SENDING : " << err_msg << std::endl;
                    replyClient(err_msg, socket);
                    return ;    
                }
                if (channel->getKey() != "")
                {
                    std::cout << "THERE IS A KEY" << std::endl;
                    std::string mdp = channel->getKey();
                    std::cout << "mdp enregistre : " << mdp << " ET : " << key << std::endl;
                    if (mdp != key)
                    {
                        std::string err_msg = ERR_BADCHANNELKEY(_clients[socket]->getNickName(), channelName);
                        std::cout << "SENDING : " << err_msg << std::endl;
                        replyClient(err_msg, socket);
                        return ;
                    }
                }
                if ((static_cast<size_t>(channel->getLimit())) < (channel->getClientlst().size() + 1))
                {
                    std::cout << (static_cast<size_t>(channel->getLimit())) << " and " << channel->getClientlst().size() + 1 << std::endl;
                    std::string err_msg = ERR_CHANNELISFULL(_clients[socket]->getNickName(), channelName);
                    std::cout << "SENDING : " << err_msg << std::endl;
                    replyClient(err_msg, socket);
                    return ;
                }
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
        {
         std::cout << "DIRECTEMENT RENTRE LA DEDEANS << <<< " << std::endl;
            createChannel(channelName, socket);
        }
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