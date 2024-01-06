CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98
DEBUGFLAGS:= -ggdb3 -fsanitize=address -D__DEBUG__

RM:= rm -rf

INCLUDES:= -I./includes

SRCS_DIR:= srcs
SRCS:= Socket.cpp TCPSocket.cpp Server.cpp 

# tester mains
TEST_SOCK:= $(SRCS_DIR)/SOCKET_main.cpp
TEST_SERVER:= $(SRCS_DIR)/SERVER_main.cpp

OBJS_DIR:= objs
OBJS:= $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

NAME:= libwebserv.a

all: $(NAME)

run: re
	./$(NAME)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

$(NAME): $(OBJS) 
	ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $@

$(OBJS_DIR):
	@mkdir -p objs

# tests
test_socket: $(TEST_SOCK) debug 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsanitize=address $(TEST_SOCK) $(NAME) -o $@

test_server: $(TEST_SOCK) debug 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsanitize=address $(TEST_SERVER) $(NAME) -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) test_socket
	$(RM) test_server

re: fclean all

.PHONY: clean fclean all re test_socket test_server

