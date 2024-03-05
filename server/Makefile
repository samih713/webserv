###	COLORS ####
BLUE:= \033[1;34m
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98 -MMD -MP
DEBUGFLAGS:= -ggdb3 -D__DEBUG__ -fsanitize=address

ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

RM:= rm -rf

INCLUDES:= -I./ -Isocket/ -I../includes/ -I../http/ -I../http/request/ -I../http/response/

SRCS:= socket/Socket.cpp socket/TCPSocket.cpp Server.cpp

OBJS_DIR:= objects
OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TEST_DIR:= tester
TEST_SOCKET:= $(TEST_DIR)/SOCKET_main.cpp
TEST_SERVER:= $(TEST_DIR)/SERVER_main.cpp

# http request/response directory
LIBHTTPDIR:= ../http/
LIBHTTP:= -L$(LIBHTTPDIR) -lhttp

DEP:= $(OBJS:%.o=%.d)

NAME:= libserver.a

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready.\n"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p objects objects/socket

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

#libs
$(LIBHTTP):
	@make -sC $(LIBHTTPDIR) debug

# tests
client: debug
	@$(CXX) $(CXXFLAGS) -fsanitize=address $(INCLUDES) $(TEST_DIR)/ClientTest.cpp $(NAME) -o $@
	@mv *.d objects/
	@echo "$(BLUE)[ TEST ]$(RESET) Client ready."

test_socket: $(TEST_SOCKET) client
	@$(CXX) $(CXXFLAGS) -fsanitize=address $(INCLUDES) $(TEST_SOCKET) $(NAME) -o $@
	@mv *.d objects/
	@echo "$(BLUE)[ TEST ]$(RESET) Socket is now ready for testing."

test_server: debug $(LIBHTTP)
	@$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(INCLUDES)  $(TEST_SERVER) $(NAME) $(LIBHTTP) -o $@
	@mv *.d objects/
	@echo "$(BLUE)[ TEST ]$(RESET) Server is now ready for testing."

tests: test_socket test_server
	@echo ""

clean:
	@$(RM) $(OBJS_DIR) *.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@make fclean -sC ../http/ > /dev/null 2>&1
	@$(RM) test_socket test_server client
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_socket test_server debug client tests
