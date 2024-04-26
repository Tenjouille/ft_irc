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
    std::string userName2 = locate.substr(d_end, (d_start - d_end) + 1);
    size_t c_start = locate.find(":");
    std::string comment;
    if (c_start != std::string::npos)
    {
        comment = "\x1b[31m\x1b[1m" + locate.substr(c_start + 1) + "\x1b[0m";
        std::cout << BLUE << "COMMENT FILLED WITH : '" << comment << "' ->" << comment.length() << RESET << std::endl;
    }

    std::cout << "CHANNEL NAME : '" << channelName << "'" << std::endl;

    // NEED MORE PARAMS IN COMMAND
    if(channelName.empty() || userName2.empty())
    {
        replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "KICK"), socket);
        return;
    }

    // CHANNEL NOT FOUND
    if (findChannel(channelName) == _channelLst.end())
    {
        replyClient(ERR_NOSUCHCHANNEL(channelName), socket);
        return;
    }

    // KICKED CLIENT IS NOT IN CHANNEL
    if (is_in_channel(userName2, channelName) == 1)
    {
        replyClient(ERR_NOTINCHANNEL(getClient(socket)->getNickName(), userName2, channelName), socket);
        return;
    }
    // KICKEUR CLIENT IS NOT IN CHANNEL
    if (is_in_channel(getClient(socket)->getNickName(), channelName) == 1)
    {
        replyClient(ERR_NOTONCHANNEL(getClient(socket)->getNickName(), channelName), socket);
        return;
    }
    
    // GET THE CHANNEL CLASS TO CHECK IF THE KICKEUR IS OP
    Channel *channel = getChannelFromName(channelName);
    if (channel == NULL)
    {
        std::cout << YELLOW << "COULDNT FIND THE CHANNEL NAME" << RESET << std::endl;
        std::string err_msg = ERR_NOSUCHCHANNEL(channelName);
        std::cout << "SENDING : " << err_msg << std::endl;
        replyClient(err_msg, socket);
        return ;
    }
    else if (isClientOp(channel->getOperatorList(), socket) == false)
    {
        std::cout << "USER : " << _clients[socket]->getNickName() << " is not an OPERATOR !!" << std::endl;
        std::string err_msg = ERR_CHANOPRIVSNEEDED(_clients[socket]->getNickName(), channelName);
        std::cout << "SENDING : " << err_msg << std::endl;
        replyClient(err_msg, socket);
        return ;
    }
    std::cout << "USER SHOULD GET KICKED" << std::endl;
    //13 because of colors
    if (comment.length() == 13)
    {
        comment.clear();
        std::cout << "NEED TO FILL THE COMMENT TO KIK" << std::endl;
        std::string color = "\x1b[1m\x1b[31m";
        comment = color + "BE GONE FOOL\x1b[0m";
    }
    std::string msg = KICK(userID(getClient(socket)->getNickName(), getClient(socket)->getUserName()), channelName, userName2, comment);
    std::cout << "Sending : " << msg << std::endl;

    replyClient(msg, getSocketFromUser(userName2));
    replyClient(msg, socket);
    findChannel(channelName)->second->removeClientFromLst(userName2);


    std::string kicked_msg = "You were kicked by " + getClient(socket)->getNickName() + "! You need to \x1b[4m\x1b[1mbehave\x1b[0m.\nReason : \"" + comment + "\"\r\n";
    std::cout << "kicked_msg : '" << kicked_msg << "'" << std::endl; 
    replyClient(kicked_msg, getSocketFromUser(userName2));
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