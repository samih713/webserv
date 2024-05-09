# main makefile

###	COLORS ####
include .colors.mk

### COMPILER SETTINGS ###
CXX:= c++
DEPFLAGS:= -MMD -MP
CXXFLAGS:= -Wall -Wextra -Werror -std=c++98
DEBUGFLAGS:= -ggdb3 -D__DEBUG__
SANITIZE:= -fsanitize=address

### OS DETECTION ###
ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

### CLEANING ###
RM:= rm -rf

### DIRECTORIES ###
SRCS_DIR:= sources
OBJS_DIR:= objects
DEPS_DIR:= deps
TESTS_DIR:= .tests
PARSER_DIR:= $(SRCS_DIR)/parser
HTTP_DIR:= $(SRCS_DIR)/http
SERVER_DIR:= $(SRCS_DIR)/server
CGI_DIR:= $(SRCS_DIR)/CGI

### EXECUTABLE ###
NAME:= webserv

### MODULES & INCLUDES ###
MODULES:= $(PARSER_DIR) $(HTTP_DIR) $(SERVER_DIR) $(CGI_DIR)
SUB_DIRS:= $(shell find $(SRCS_DIR) -type d -name $(TESTS_DIR) -prune -o -type d -print)
INCLUDES:= -I./includes/ $(patsubst %,-I./%,$(MODULES)) $(patsubst %,-I./%,$(SUB_DIRS))

### SOURCES ###
include $(patsubst %,%/module.mk,$(MODULES))
SRCS += $(SRCS_DIR)/main.cpp

### OBJECTS ###
OBJS:= $(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) $(OBJS) -o $@
	@echo "$(YELLOW)$(BOLD)[ EXECUTABLE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)$(BOLD)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(SUB_DIRS))

run: re
	./$(NAME) configs/webserv.conf

debug: export CXXFLAGS += $(DEBUGFLAGS)
debug: fclean
	@$(MAKE) -se all
	@echo "$(MAGENTA)$(BOLD)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) $(OBJS_DIR); \
		echo "$(RED)$(BOLD)[ DELETE ]$(RESET) Removed object files."; \
	fi
	@if [ -f $(TEST_PARSER) ] || [ -f $(TEST_HTTP) ] || [ -f $(TEST_SOCKET) ] || [ -f $(TEST_CGI) ] || [ -f client ]; then \
		$(RM) $(TEST_PARSER) $(TEST_HTTP) $(TEST_SOCKET) $(TEST_CGI) client; \
		echo "$(RED)$(BOLD)[ DELETE ]$(RESET) Removed testers."; \
	fi

fclean: clean
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		echo "$(RED)$(BOLD)[ DELETE ]$(RESET) Removed $(NAME)."; \
	fi

re: fclean
	@$(MAKE) -s all

test_parser:
	@cd ./$(PARSER_DIR)/$(TESTS_DIR) && ./run_tests.sh && cd -

test_http:
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(CGI_SRCS) $(HTTP_SRCS) $(TEST_HTTP_SRC) -o $(TEST_HTTP)
	@echo "$(BLUE)$(BOLD)[ TEST ]$(RESET) HTTP ready for testing."

#! SOCKET_main.cpp has a compile error so this test has been commented out
# test_socket:
# 	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SOCKET_SRCS) $(TEST_CLIENT_SRC) -o $(SERVER_DIR)/client @$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SOCKET_SRCS) $(TEST_SOCKET_SRC) -o $(TEST_SOCKET)
# 	@echo "$(BLUE)$(BOLD)[ TEST ]$(RESET) SOCKET ready for testing."

client: $(TEST_CLIENT_SRC)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(TEST_CLIENT_SRC) $(SERVER_DIR)/socket/Socket.cpp $(SERVER_DIR)/socket/TCPSocket.cpp -o $@
	@echo "$(BLUE)$(BOLD)[ TEST ]$(RESET) Client is ready."

test_cgi:
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SANITIZE) $(HTTP_SRCS) $(CGI_SRCS) $(TEST_CGI_SRC) -o $(TEST_CGI)
	@echo "$(BLUE)$(BOLD)[ TEST ]$(RESET) CGI ready for testing."

format:
	@echo "$(BLUE)$(BOLD)[ FORMAT ]$(RESET) Formatting code..."
	@find . -name "*.cpp" -exec clang-format -i {} +
	@find . -name "*.hpp" -exec clang-format -i {} +
	@echo "$(BLUE)$(BOLD)[ FORMAT ]$(RESET) Code has been formatted."

-include $(OBJS:.o=.d)

.PHONY: clean fclean all re debug run test_http test_socket test_cgi format
