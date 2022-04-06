#ifndef LOGIC_CHANNEL_HPP
#define LOGIC_CHANNEL_HPP
#include "ircserv.hpp"
#include "Logic_User.hpp"
#include "User.hpp"

	class Logic_Channel
	{
		public:
			Logic_Channel();
			~Logic_Channel();
			static void			sendMessageToChannel(Channel *channel, User *sender, std::string message, std::string commandName);
	};

#endif