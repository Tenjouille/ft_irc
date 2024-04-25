#include "Server.hpp"

void Server::kickCmd(std::string locate, int socket)
{
    std::cout << "locate = '" << locate << "'" << std::endl;
    size_t d_start = locate.find(":") - 2;
    size_t d_end = locate.rfind(" ", d_start) + 1;
   // std::cout << "locatend = " << locate[d_end] << std::endl;
 //   std::cout << "locatestart = " << locate[d_start] << std::endl;
    size_t start = locate.find("#");
    std::string channelName;
    size_t end;
    std::string end_bis;
    if (start != std::string::npos)
    {
        end = locate.find_first_of(" ", start);
        if (end != std::string::npos)
        {
            channelName = locate.substr(start + 1, end - start - 1);
        }
    }
   // std::string userName = locate.substr(d_end, (d_end - d_start));
    std::string userName2 = locate.substr(d_end, (d_start - d_end) + 1);
    //std::cout << "1 = '" << userName << "' 2 = '" << userName2 << "'" << std::endl;
    size_t c_start = locate.find(":");
    std::string comment;
    if (c_start != std::string::npos)
    {
        comment = locate.substr(c_start + 1);
    }
    if (findChannel(channelName) != _channelLst.end())
    {
        //findChannel(channelName)->second->removeClientFromLst(channelName);
        findChannel(channelName)->second->removeClientFromLst(userName2);
    }
    if(channelName.empty() || userName2.empty())
    {
        replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "KICK"), socket);
        return;
    }
    if (findChannel(channelName) == _channelLst.end())
    {
        replyClient(ERR_NOSUCHCHANNEL(channelName), socket);
        return;
    }
    if (is_in_channel(userName2, channelName) == 1)
    {
        replyClient(ERR_NOTINCHANNEL(getClient(socket)->getNickName(), userName2, channelName), socket);
    }
    if (is_in_channel(getClient(socket)->getNickName(), channelName) == 1)
    {
        replyClient(ERR_NOTONCHANNEL(getClient(socket)->getNickName(), channelName), socket);
    }
    std::cout << RED << "le client :" << YELLOW << getClient(socket)->getNickName() << RED << "veut kick le user : " << YELLOW << userName2 << RED << "du channel : '" << YELLOW << channelName << "'" << std::endl;
//    std::cout << RED << "user: " << userName << " socket = "<< getSocketFromUser(userName2) << std::endl;
    replyClient(KICK(userID(getClient(socket)->getNickName(), getClient(socket)->getUserName()), channelName, userName2, comment), getSocketFromUser(userName2));
    replyClient(KICK(userID(getClient(socket)->getNickName(), getClient(socket)->getUserName()), channelName, userName2, comment), socket);
    //(void)socket;
}

/*int Server::is_in_channel(std::string str, std::string channel)
{
    for(std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if(it->first == channel)
        {
            for (std::map<int, Client*>::iterator it_clt = it->second->getClientlst().begin(); it_clt != it->second->getClientlst().end(); ++it_clt)
            {
                if(it_clt->second->getNickName() == str)
                    return 0;
            }
        }
    }
    return 1;
}*/

int Server::is_in_channel(std::string str, std::string channel)
{
    for(std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if(it->first == channel)
        {
            std::map<int, Client*> clientList = it->second->getClientlst(); // Stocker la liste des clients dans une variable
            for (std::map<int, Client*>::iterator it_clt = clientList.begin(); it_clt != clientList.end(); ++it_clt)
            {
                if(it_clt->second->getNickName() == str)
                    return 0;
            }
        }
    }
    return 1;
}

    //si pas operateur renvoyer erreur
    //si le client n est pas dans le channel 
    //si le user mentionnee n est pas dans le channel
    //  "<client> <nick> <channel> :They aren't on that channel"
    //"<client> <channel> :You're not on that channel"
    //  "<client> <channel> :You're not channel operator"