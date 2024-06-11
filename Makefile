# main makefile

###	COLORS ####
include .colors.mk

### COMPILER SETTINGS ###
CXX:= c++
DEPFLAGS:= -MMD -MP
DEBUGFLAGS:= -ggdb3 -D__DEBUG__
SANITIZE:= -fsanitize=address
CXXFLAGS:= -Wall -Wextra -Werror -std=c++98

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

log: export CXXFLAGS += -D__LOG_TO_FILE__
log: fclean
	@$(MAKE) -se all
	@echo "$(MAGENTA)$(BOLD)[ LOGGING ]$(RESET) $(NAME) is logging to file."

debug: export CXXFLAGS += $(DEBUGFLAGS)
debug: fclean
	@$(MAKE) -se all
	@echo "$(MAGENTA)$(BOLD)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) $(OBJS_DIR); \
		echo "$(RED)$(BOLD)[ DELETE ]$(RESET) Removed object files."; \
	fi
	@if [ -f $(TEST_PARSER) ] || [ -f $(TEST_SOCKET) ] || [ -f client ]; then \
		$(RM) $(TEST_PARSER) $(TEST_SOCKET) client; \
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

test_socket:
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SOCKET_SRCS) $(TEST_CLIENT_SRC) -o $(SERVER_DIR)/client @$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SOCKET_SRCS) $(TEST_SOCKET_SRC) -o $(TEST_SOCKET)
	@echo "$(BLUE)$(BOLD)[ TEST ]$(RESET) SOCKET ready for testing."

client: $(TEST_CLIENT_SRC)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(TEST_CLIENT_SRC) $(SERVER_DIR)/socket/Socket.cpp $(SERVER_DIR)/socket/TCPSocket.cpp -o $@
	@echo "$(BLUE)$(BOLD)[ TEST ]$(RESET) Client is ready."

format:
	@echo "$(BLUE)$(BOLD)[ FORMAT ]$(RESET) Formatting code..."
	@find . -name "*.cpp" -exec clang-format -i {} +
	@find . -name "*.hpp" -exec clang-format -i {} +
	@echo "$(BLUE)$(BOLD)[ FORMAT ]$(RESET) Code has been formatted."

-include $(OBJS:.o=.d)

.PHONY: all run log debug clean fclean re test_parser test_socket client format
