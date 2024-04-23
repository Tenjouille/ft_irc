#include "Server.hpp"

void Server::kickCmd(std::string locate, int socket)
{
    //KICK #channelname user :comment

    std::cout << "locate = '" << locate << "'" << std::endl;
    size_t d_start = locate.find(":");
    std::string username;
    if (d_start != std::string::npos)
    {
        // Trouver la position du premier espace après le ":"
        size_t space_pos = locate.find(" ", d_start);
        if (space_pos != std::string::npos)
        {
            // Extraire la sous-chaîne entre le ":" et le premier espace trouvé après le ":"
            username = locate.substr(d_start + 1, space_pos - d_start - 1);
        }
    }
    size_t start = locate.find("#");
    std::string channelName;
    if (start != std::string::npos)
    {
        size_t end = locate.find_first_of(" ", start);
        if (end != std::string::npos)
        {
            channelName = locate.substr(start, end - start);
        }
    }
    size_t c_start = locate.find(":");
    std::string comment;
    if (c_start != std::string::npos)
    {
        comment = locate.substr(c_start + 1);
    }
    if (findChannel(channelName) != _channelLst.end())
    {
        findChannel(channelName)->second->removeClientFromLst(channelName);
    }
    replyClient(KICK(userID(getClient(socket)->getNickName(), getClient(socket)->getUserName()), channelName, username, comment), getSocketFromUser(username));
    //(void)socket;
}
