define ASCIIART
 
          -~~~~-
         |O __ O|
      -__|_\__/_|__-
      ---(__/\__)---
        /||~--~||\\
       / ||    || \\
      |  `'    `'  |
    _/ /\\__    __/\\ \_
  ~~~~~    ~~~~    ~~~~~ 

endef
export ASCIIART

NAME = webserver
ORANGE = "\033[33m"
WHITE = "\033[0;37m" 

CLASS = ./srcs/classes/

CXX = c++ $(EFLAGS)
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra -g3 $(EFLAGS)
RM = rm -rf
SANI := -fsanitize=address -g3

SRCS = main.cpp $(CLASS)Connection.cpp $(CLASS)Port.cpp $(CLASS)Client.cpp $(CLASS)HttpRequest.cpp $(CLASS)HttpResponse.cpp $(CLASS)Location.cpp $(CLASS)Server.cpp $(CLASS)WebServer.cpp $(CLASS)Kqueue.cpp $(CLASS)Server_utils.cpp $(CLASS)Config.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: all
	@./$(NAME)

sani:
	$(MAKE) EFLAGS='$(SANI)' all

val: all
	@valgrind --leak-check=yes ./$(NAME)
	rm -rf $(NAME).dSYM

clean:
	rm -f $(OBJS) $(TARGET)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: test sani val clean fclean
