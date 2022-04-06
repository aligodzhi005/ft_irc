NAME	=	ircserv
FLAGS	=	-std=c++98
SRC		=	sources/Channel.cpp \
			sources/Commands.cpp \
			sources/Server.cpp \
			sources/User.cpp \
			sources/Utils.cpp \
			sources/Logic_User.cpp \
			sources/Logic_Channel.cpp \
			sources/main.cpp
OBJ		=	$(SRC:.cpp=.o)
INC		=	includes/Channel.hpp \
			includes/Commands.hpp \
			includes/Server.hpp \
			includes/User.hpp \
			includes/Utils.hpp \
			includes/Logic_User.hpp \
			includes/Logic_Channel.hpp \
			includes/ircserv.hpp

all: $(NAME)

%.o: %.cpp $(INC)
	clang++ $(FLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	clang++ $(FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)
	@rm -f */*.out
	@rm -f *.out

fclean: clean
	@rm -f $(NAME)

re: fclean all

run: $(NAME)
	@clear
	@./$(NAME) 127.0.0.1:6667:69

client:
	@clear
	@nc -c 127.0.0.1 6667

.PHONY: all clean fclean re run client
