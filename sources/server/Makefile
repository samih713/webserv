# server makefile

include ../../.common.mk

NAME:= libserver.a

INCLUDES:= -I./ -I./socket/ -I./connection_manager/ -I../includes/

SRCS:= socket/Socket.cpp socket/TCPSocket.cpp connection_manager/ConnectionManager.cpp Server.cpp

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TESTS_DIR:= .tests
TEST_SOCKET:= $(TESTS_DIR)/SOCKET_main.cpp

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(addprefix $(OBJS_DIR)/,socket connection_manager)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(NAME)
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

# tests
client: debug
	@$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES) $(TESTS_DIR)/ClientTest.cpp $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Client ready."

test_socket: $(TEST_SOCKET) client
	@$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_SOCKET) $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Socket is now ready for testing."

tests: test_socket
	@echo ""

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) $(OBJS_DIR); \
		echo "$(RED)[ DELETE ]$(RESET) Removed object files."; \
	else \
		echo "$(GREEN)[ CLEAN ]$(RESET) No object files to remove."; \
	fi

fclean: clean
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME)."; \
	else \
		echo "$(GREEN)[ FCLEAN ]$(RESET) No $(NAME) to remove."; \
	fi
	@$(RM) test_socket client

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_socket debug client tests
