#include "Server.hpp"

bool	Server::fillinBuffer(std::string locate, std::string& channelname, std::string& invited, std::string& nickname, int socket)
{
	Client *sender = getClient(socket);
	if (sender == NULL || !sender)
	{
		return (false);
	}
	nickname = sender->getNickName();
	if (nickname.empty())
	{
		return (false);
	}
    channelname.clear();
    invited.clear();
    size_t channel_start = locate.find("#");
    if (channel_start != std::string::npos)
    {
        size_t channel_end = locate.find(" ", channel_start);
        if (channel_end != std::string::npos)
        {
            channelname = locate.substr(channel_start, channel_end - channel_start);
        }
        else
        {
            channelname = locate.substr(channel_start);
        }
    }
	size_t space_pos = locate.find(" ");
	if (space_pos != std::string::npos)
	{
    	invited = locate.substr(space_pos + 1);
    	size_t end_pos = invited.find(" ");
    	if (end_pos != std::string::npos)
    	{
    	    invited = invited.substr(0, end_pos);
    	}
	}
	if (channelname.empty())
		return (false);
	if (invited.empty())
		return (false);
	if (nickname.empty())
		return (false);

    return true;
}

void	Server::inviteCmd(std::string locate, int socket)
{
	std::string invited_nickname;
	std::string channel_name;
	std::string nickname;

	if (fillinBuffer(locate, channel_name, invited_nickname, nickname, socket) == true)
	{
	}
	else
	{
		return ;
	}
	Client *sender = getClient(socket);
	sender->printChannels();
	std::map<std::string, Channel*>::iterator it1 = _channelLst.begin();
	std::map<std::string, Channel*>::iterator ite1 = _channelLst.end();
	bool	channel_exist = false;
	bool	already_on = false;
	std::string tmp_username_invited;
	while (it1 != ite1)
	{
		std::string tmp_name = "#" + it1->first;
		if (channel_name == tmp_name)
		{
			channel_exist = true;
			std::map<int, Client*> client_list = it1->second->getClientlst();
			std::map<int, Client *>::iterator first = client_list.begin();
			std::map<int, Client *>::iterator end = client_list.end();
			while (first != end)
			{
				if (first->second->getNickName() == invited_nickname)
				{
					already_on = true;
					tmp_username_invited = first->second->getUserName();
					break;
				}
				++first;
			}
			break ;
		}
		++it1;
	}
	if (channel_exist == false)
	{
		std::string doesnt_exist = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channel_name);
		replyClient(doesnt_exist, socket);
		return ;
	}
	if (already_on == true)
	{
		std::string already_on = ERR_USERONCHANNEL(tmp_username_invited, nickname, channel_name);
		replyClient(already_on, socket);
		return ;
	}
	std::vector<std::string>channels = sender->getChannel();
	std::vector<std::string>::iterator it = channels.begin();
	std::vector<std::string>::iterator itend = channels.end();
	bool	sender_in_channel = false;
	while (it != itend)
	{
		std::string tmp_name = "#" + * it;
		if (channel_name == tmp_name)
		{
			sender_in_channel = true;
			break;
		}
		++it;
	}
	if (sender_in_channel == false)
	{
		std::string error_msg = ERR_NOTONCHANNEL(nickname, channel_name);
		replyClient(error_msg, socket);
		return ;
	}
	else if (sender_in_channel == true)
	{
		std::cout << "ici2" << std::endl;
		std::string error_msg = ERR_CHANOPRIVSNEEDED(nickname, channel_name);
		replyClient(error_msg, socket);
		return ;
	}
	std::string username = sender->getUserName();
	std::string userID = ":" + nickname + "!" + username + "@localhost";
	std::string msg = RPL_INVITING(userID,  nickname, invited_nickname, channel_name);
	int socket_to_send_to = getSocketFromUser(invited_nickname);
	replyClient(msg, socket_to_send_to);
	return ;
}