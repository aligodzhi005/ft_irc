#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

class Server;

# include <vector>
# include "ircserv.hpp"
# include "User.hpp"
# include "Utils.hpp"
# include "Channel.hpp"
# include "Commands.hpp"
# include <poll.h>

# define MAX_CONNECTION				2000
# define RECV_FLAGS					0


class Server
{
	private:
		int						_sock;
		const std::string		_host;
		const std::string		_port;
		const std::string		_password;
		std::string				_serverName;
		std::vector<pollfd>		_pollfds;
		std::vector<User *>		_users;
		std::vector<Channel *>	_channels;
		Commands					*_Commands;

		Server();
		int				        createSocket();
		int				        recvMsg(User *user);

	public:
		Server(const std::string host, const std::string port, const std::string password);
		~Server();
        void			        removeUser(std::string id);
        int				        acceptUser();
        User			        *getUser(std::string userName);
        User			        *getUserById(std::string id);
        void                    removeUserFromPoll(std::string id);
        void                    removeUserFromUsers(std::string id);
		std::string				getPrefix() const;
        std::vector<Channel*>   getChannels();
        Channel			        *getChannel(std::string chanName);
        Channel			        *createChannel(std::string name, User *admin);
		void					deleteChannel(Channel *channel);

		void			        start();
		void			        greeting(int client_d)const;
		void			        action();
		bool			        checkPassword(std::string userPassword);
		void					sendMessage(User* to, std::string message);
};

#endif
