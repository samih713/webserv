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
INCLUDES:= -I./includes/ $(patsubst %,-I./%,$(MODULES))

### SOURCES ###
SRCS:= $(SRCS_DIR)/main.cpp

### OBJECTS & SUBDIRS ###
include $(patsubst %,%/module.mk,$(MODULES))
OBJS += $(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
SUB_DIRS:= $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(shell find $(SRCS_DIR) -type d))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) $(OBJS) -o $@
	@echo "$(YELLOW)[ EXECUTABLE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $@ $(SUB_DIRS)

run: re
	./$(NAME) configs/webserv.conf

debug: export CXXFLAGS += $(DEBUGFLAGS)
debug: fclean
	@$(MAKE) -se all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) $(OBJS_DIR); \
		echo "$(RED)[ DELETE ]$(RESET) Removed object files."; \
	fi
	@if [ -f $(TEST_PARSER) ] || [ -f $(TEST_HTTP) ] || [ -f $(TEST_SOCKET) ]; then \
		$(RM) $(TEST_PARSER) $(TEST_HTTP) $(TEST_SOCKET); \
		echo "$(GREEN)[ DELETE ]$(RESET) Removed testers."; \
	fi

fclean: clean
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME)."; \
	fi

re: fclean
	@$(MAKE) -s all

test_parser:
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(PARSER_SRCS) $(TEST_PARSER_SRC) -o $(TEST_PARSER)
	@echo "$(BLUE)[ TEST ]$(RESET) Parser ready for testing."

test_http:
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(HTTP_SRCS) $(TEST_HTTP_SRC) -o $(TEST_HTTP)
	@echo "$(BLUE)[ TEST ]$(RESET) HTTP ready for testing."

#! SOCKET_main.cpp has a compile error so this test has been commented out
# test_socket:
# 	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SOCKET_SRCS) $(TEST_CLIENT_SRC) -o $(SERVER_DIR)/client
# 	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUGFLAGS) $(SOCKET_SRCS) $(TEST_SOCKET_SRC) -o $(TEST_SOCKET)
# 	@echo "$(BLUE)[ TEST ]$(RESET) SOCKET ready for testing."

# test_cgi: fclean debug
# 	@make -C ../sources/HTTP
# 	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SANITIZE) $(INCLUDES) $(libhttp) $(TEST_CGI) $(NAME) -o $@
# 	@mv *.d $(OBJS_DIR)
# 	@echo "$(BLUE)[ TEST ]$(RESET) CGI is now ready for testing."

-include $(OBJS:.o=.d)

.PHONY: clean fclean all re debug run test_parser test_http test_socket
