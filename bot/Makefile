NAME 		= 	kenny

CC 	 		= 	c++
SRCS 		= 	Bot.cpp \
				main.cpp
HEAD 		= 	Bot.hpp
MFILE   	= 	Makefile
FLAGS 		= 	-Wall -Wextra -Werror
STANDART 	= 	-std=c++98

OBJS_PATH 	= 	objects/
OBJ 		= 	$(SRCS:.cpp=.o)
OBJS 		= 	$(addprefix $(OBJS_PATH), $(OBJ))

$(OBJS_PATH)%.o:%.cpp
	@mkdir -p $(OBJS_PATH)
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) $(HEAD) $(MFILE)
	@$(CC) $(FLAGS) $(STANDART) -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	@rm -rf $(OBJS_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all

bot: all
	@clear
	@./$(NAME) 6667 ""

.PHONY: all clean fclean re run
