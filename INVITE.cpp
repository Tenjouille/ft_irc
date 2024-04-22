#include "Server.hpp"

bool	Server::fillinBuffer(std::string locate, std::string& channelname, std::string& invited, std::string& nickname, int socket)
{
	Client *sender = getClient(socket);
	
	if (sender == NULL || !sender)
	{
		std::cout << "COULDNT GET CLIENT FROM SOCKET" << std::endl;
		return (false);
	}

	nickname = sender->getNickName();
	if (nickname.empty())
	{
		std::cout << "NICKNAME IS EMPTY" << std::endl;
		return (false);
	}

    // Clear the output strings
    channelname.clear();
    invited.clear();

    // Find the position of '#' and the next space
    size_t channel_start = locate.find("#");
    if (channel_start != std::string::npos)
    {
        size_t channel_end = locate.find(" ", channel_start);
        if (channel_end != std::string::npos)
        {
            // Extract the channel name substring
            channelname = locate.substr(channel_start, channel_end - channel_start);
        }
        else
        {
            // If no space is found after '#', consider the rest of the string as the channel name
            channelname = locate.substr(channel_start);
        }
    }

	// Find the position of the first space
	size_t space_pos = locate.find(" ");
	if (space_pos != std::string::npos)
	{
    	// Extract the substring from the first space to the end of the string
    	invited = locate.substr(space_pos + 1);

    	// Find the end of the invited nickname
    	size_t end_pos = invited.find(" ");
    	if (end_pos != std::string::npos)
    	{
    	    // Extract the invited nickname substring
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
		std::cout << "BUFFERS ARE FILLED" << std::endl;
		std::cout << "Channel name : '" << channel_name << "'" << "\n" \
		<< "Invited nickname : '" << invited_nickname << "'" << "\n" \
		<< "InvitEUR nickname : '" << nickname << "'" << std::endl; 
	}
	else
	{
		std::cout << RED << "FAILED TO FILL IN BUFFERS" << RESET << std::endl;
		//IDK WHAT TO DO BUT NEED TO DO MORE THAN THAT IM GUESSING?
		return ;
	}

	/*PRINTING DES MACHINS POUR DEBUG*/
	Client *sender = getClient(socket);
	std::cout << "PRINTING CHANNELS" << std::endl;
	sender->printChannels();
	std::cout << "INVINTING FOR : " << channel_name << std::endl;
	std::cout << "NBB CCHANNELS : " << _nb_channels << std::endl;
	std::cout << "\n\n";

	//CHECK THAT THE CHANNEL EXISTS
	std::map<std::string, Channel*>::iterator it1 = _channelLst.begin();
	std::map<std::string, Channel*>::iterator ite1 = _channelLst.end();
	bool	channel_exist = false;
	bool	already_on = false;
	std::string tmp_username_invited;
	while (it1 != ite1)
	{
		std::string tmp_name = "#" + it1->first;
		std::cout << "Chann : " << tmp_name << std::endl;
		std::cout << YELLOW << "COMPARING : " << channel_name << " et " << tmp_name << RESET << std::endl;
		if (channel_name == tmp_name)
		{
			std::cout << "CHANNEL EXISTS : " << tmp_name << std::endl;
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

	//CHANNEL DOESNT EXISTS
	if (channel_exist == false)
	{
		std::string doesnt_exist = ERR_NOSUCHCHANNEL(channel_name);
		std::cout << YELLOW << "CHANNEL DOESNT EXIST" << RESET << std::endl;
		std::cout << RED << "SENDING ERROR : " << doesnt_exist << RESET << std::endl;
		replyClient(doesnt_exist, socket);
		return ;
	}

	//CHECK THAT THE INVITED IS NOT ALREADY INSIDE CHANNEL
	if (already_on == true)
	{
		//! NEED TO DETERMINE WHAT CLIENt IS FOR NOW USERNAME??? !//
		std::string already_on = ERR_USERONCHANNEL(tmp_username_invited, nickname, channel_name);
		std::cout << YELLOW << "ALREADY ON" << RESET << std::endl;
		std::cout << RED << "SENDING ERROR : " << already_on << RESET << std::endl;
		replyClient(already_on, socket);
		return ;
	}

	//CHECK THAT THE INVITEUUUR IS ON THE CHANNEL
	std::cout << "\n\n";
	std::vector<std::string>channels = sender->getChannel();
	std::vector<std::string>::iterator it = channels.begin();
	std::vector<std::string>::iterator itend = channels.end();
	bool	sender_in_channel = false;
	while (it != itend)
	{
		std::string tmp_name = "#" + * it;
		std::cout << "Chann : " << tmp_name << std::endl;
		std::cout << YELLOW << "COMPARING : " << channel_name << " et " << tmp_name << RESET << std::endl;
		if (channel_name == tmp_name)
		{
			sender_in_channel = true;
			break;
		}
		++it;
	}
	
	//CHANNEL WAS NOT FOUND IN THE CLIENT'S CHANNEL CONTAINER
	if (sender_in_channel == false)
	{
		std::string error_msg = ERR_NOTONCHANNEL(nickname, channel_name);
		std::cout << YELLOW << "SENDER IS NOT IN CHANNEL" << RESET << std::endl;
		std::cout << RED << "MSG D'ERREUR : " << error_msg << RESET << std::endl;
		replyClient(error_msg, socket);
		return ;
	}
	else if (sender_in_channel == true) //need to add the permission check
	{
		std::string error_msg = ERR_CHANOPRIVSNEEDED(nickname, channel_name);
		std::cout << YELLOW << "SENDER CANT INVITE" << RESET << std::endl;
		std::cout << RED << "MSG D'ERREUR : " << error_msg << RESET << std::endl;
		replyClient(error_msg, socket);
		return ;
	}
	//SENDING MESSAGE NO ERROR DETECTED
	std::string username = sender->getUserName();
	std::string userID = ":" + nickname + "!" + username + "@localhost";
	std::string msg = RPL_INVITING(userID,  nickname, invited_nickname, channel_name);
	int socket_to_send_to = getSocketFromUser(invited_nickname);
	std::cout << BLUE << "MSG TO SEND : " << msg << RESET << std::endl;	
	replyClient(msg, socket_to_send_to);
	
	return ;
}