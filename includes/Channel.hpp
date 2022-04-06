#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

class Channel;

# include "ircserv.hpp"
# include "User.hpp"
# include "Server.hpp"

bool isAllowedChannelName(std::string name);

class Channel
{
	private:
		std::string			_name;
		std::vector<User *>	_users;
		User*				_admin;
		Server*				_server;


		Channel();

	public:
		Channel(std::string name, User* admin, Server *server);
		~Channel();

		std::string			getName() const;
		User				*getAdmin() const;
		User				*getUser(std::string userName);
		std::vector<User *>	getUsers() const;
		void				addUser(User *newUser);
		void				removeUser(User *user);
		void				setName(std::string name);
		
		void			sendMessageToChannel(User *sender, std::string message, std::string commandName);
};

#endif
