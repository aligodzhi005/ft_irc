#ifndef LOGIC_USER_HPP
# define LOGIC_USER_HPP
#include "ircserv.hpp"
#include "Commands.hpp"
#include "User.hpp"

using namespace std;

class Logic_User
{
	private:

	public:
		static void			removeUserFromChannel(User *user);
		static void			appendMessage(string message, User *user);
		static void			clearMessage(User *user);
		static void			sendMessage(User *from, User* to, string message);
		static void			doRegister(User *user);
		static void			getReply(User *user, string message);
		Logic_User();
		~Logic_User();
};

#endif
