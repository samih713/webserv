include ../common.mk

NAME:= libserver.a

HTTP_INCLUDES:= -I../http/ -I../http/request/ -I../http/response/ -I../http/handler/
INCLUDES:= -I./ -I./socket/ -I./connection_manager/ -I../includes/  $(HTTP_INCLUDES)

SRCS:= socket/Socket.cpp socket/TCPSocket.cpp connection_manager/ConnectionManager.cpp Server.cpp

OBJS_DIR:= objs
OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TEST_DIR:= tester
TEST_SOCKET:= $(TEST_DIR)/SOCKET_main.cpp
TEST_SERVER:= $(TEST_DIR)/SERVER_main.cpp

# http request/response directory
LIBHTTPDIR:= ../http/
LIBHTTP:= -L$(LIBHTTPDIR) -lhttp

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready."

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(addprefix $(OBJS_DIR)/,socket connection_manager)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(NAME)
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

# test libs
libhttp:
	@make -sC $(LIBHTTPDIR) debug

# tests
client: debug
	@$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_DIR)/ClientTest.cpp $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Client ready."

test_socket: $(TEST_SOCKET) client
	@$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_SOCKET) $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Socket is now ready for testing."

test_server: debug libhttp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SERVER) $(NAME) $(LIBHTTP) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Server is now ready for testing."

tests: test_socket test_server
	@echo ""

clean:
	@$(RM) $(OBJS_DIR) *.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@make -sC $(LIBHTTPDIR) fclean
	@$(RM) test_socket test_server client
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_socket test_server debug client tests
