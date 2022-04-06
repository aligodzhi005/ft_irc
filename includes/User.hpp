#ifndef USER_CLASS_HPP
# define USER_CLASS_HPP

class User;

# include "ircserv.hpp"
# include "Channel.hpp"

using std::vector;
using std::string;

class User
{
	private:
		string				_id;
		bool				_didEnter;
		bool				_didRegister;
		int					_sockFd;
		int					_port;
		string				_host;
		string				_nick;
		string				_username;
		string				_message;

		// TODO: user should be able to join multiple channels
		static size_t		_maxChannels;
		vector<Channel *>	_channels;
		Channel*			_channel;

	public:

		User(int sockFd, int port);
		User(int sockFd, char *host, int port);
		~User();

		bool			didEnter() const;
		bool			getDidEnter() const;
		void			setDidEnter(bool value);
		bool			didRegister() const;
		void			setRegister(bool tmp);
		int				getSockFd() const;
		int				getPort() const;
		string			getNick() const;
		string			getHost() const;
		string			getMessage() const;
		Channel			*getChannel() const;
		string			getName() const;
		string			getPrefix() const;
		void			setNick(string nickname);
		void			setUsername(string username);
		string			getUsername() const;
		void			setChannel(Channel *channel);
		string			getId() const;
		
		string			get_id();

		void			removeUserFromChannel();
		void			appendMessage(string message);
		void			clearMessage();
		void			sendMessage(User* to, string message);
		void			doRegister();
		
		void			getReply(string message);
};

#endif
