#include "Channel.hpp"

void	Channel::changeOperator(char sign, std::vector<std::string>& args)
{
	int 								socket = -1;
	std::map<int, Client*>::iterator	it = _clientslst.begin();

	for (;it != _clientslst.end(); it++)
		if (it->second->getNickName() == args[0])
			socket = it->first;
	if (sign == '+')
	{
		std::cout << " +o";
		if (args.size())
			return ;
		newOperator(socket, it->second);
	}
	else if (args.size())
	{
		std::cout << " -o";
		_operators.erase(socket);
		if (args.size())
			args.erase(args.begin());
	}
	args.erase(args.begin());
}


void	Channel::changeInvit(char sign)
{
	if (sign == '+')
	{
		_inviteonly = true;
		std::cout << " +i";
	}
	else
	{
		_inviteonly = false;
		std::cout << " -i";
	}
}

void	Channel::changeTopic(char sign)
{
	if (sign == '+')
	{
		_topicStatus = 1;
		std::cout << " +t";
	}
	else
	{
		_topic = "";
		_topicStatus = 0;
		std::cout << " -t";
	}
}

void	Channel::changeKey(char sign, std::vector<std::string>& args)
{
	if (sign == '+')
	{
		if (!args.size())
			return ;
		_key = args[0];
		args.erase(args.begin());
		std::cout << " +k";
	}
	else
	{
		std::cout << " -k";
		_key = "";
		if (args.size())
			args.erase(args.begin());
	}
}

void	Channel::changeLimit(char sign, std::vector<std::string>& args)
{
	if (sign == '+')
	{
		_limit = atoi(args[0].c_str());
		std::cout << " +l";
		if (args.size())
			args.erase(args.begin());
	}
	else
	{
		std::cout << " -l";
		_limit = 0;
	}
}
