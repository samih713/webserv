# main makefile

###	COLORS ####
BLUE:= \033[1;34m
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

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

### EXECUTABLE ###
NAME:= webserv

### MODULES & INCLUDES ###
MODULES:= $(PARSER_DIR) $(HTTP_DIR) $(SERVER_DIR)
INCLUDES:= -I./includes/ $(patsubst %,-I./%,$(MODULES))

### SOURCES ###
SRCS:=
include $(patsubst %,%/module.mk,$(MODULES))
SRCS_LIST += main.cpp
SRCS += $(patsubst %,$(SRCS_DIR)/%,$(SRCS_LIST))

### OBJECTS & SUBDIRS ###
OBJS:= $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
SUB_DIRS:= $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(shell find $(SRCS_DIR) -type d))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@
	@echo "$(YELLOW)[ EXECUTABLE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR) $(SUB_DIRS)

debug: export CXXFLAGS += $(DEBUGFLAGS)
debug: fclean
	@$(MAKE) -se all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

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

re: fclean
	@$(MAKE) all

-include $(OBJS:.o=.d)

.PHONY: clean fclean all re debug run
