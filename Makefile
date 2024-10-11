END			:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
MAGENTA		:= \033[1;35m
CYAN		:= \033[1;36m
WHITE		:= \033[1;37m

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++98

OBJSDIR		:= objs
SRCS		:= $(shell find -name '*.cpp')
OBJS		:= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))
NAME		:= webserv

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)stuff compiled$(END) 💻\n"

$(OBJSDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJSDIR)

fclean: clean
	@rm -f $(NAME)
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)stuff removed$(END) 🗑️\n"

re: fclean all

VARS := 

run: re
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)started$(END) 🟢\n"
	@./$(NAME) $(VARS)

detached: re
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)started detached$(END) 🟣\n"
	@./$(NAME) $(VARS) &

stop:
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)stopped$(END) 🔴\n"
	@kill $$(pgrep $(NAME))

reload: re stop detached

val: re
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)started with valgrind$(END) ⚫\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./$(NAME) $(VARS)

p: re
	@touch index.html
	@clear && valgrind --leak-check=full ./$(NAME) --verbose ./config_files/parse.conf

.PHONY: all clean fclean re run p
