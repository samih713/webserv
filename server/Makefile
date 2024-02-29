include ../common.mk

INCLUDES:= -I./ -Isocket/ -I../includes/ -I../http/ -I../http/request/ -I../http/response/

SRCS:= socket/Socket.cpp socket/TCPSocket.cpp Server.cpp

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
	@make -sC $(LIBHTTPDIR)

# tests
client: debug
	@$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_DIR)/ClientTest.cpp $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Client ready."

test_socket: $(TEST_SOCKET) client
	@$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_SOCKET) $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Socket is now ready for testing."

test_server: debug $(NAME) $(TEST_SOCKET) $(LIBHTTP) 
	@$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES)  $(TEST_SERVER) $(NAME) $(LIBHTTP) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Server is now ready for testing."

tests: test_socket test_server
	@echo ""

clean:
	@$(RM) $(OBJS_DIR) *.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@$(RM) test_socket test_server client
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_socket test_server debug client tests
