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
CXXFLAGS:= -Wall -Wextra -Werror -std=c++98 $(DEPFLAGS)
DEBUGFLAGS:= -ggdb3 -D__DEBUG__
SANITIZE:= -fsanitize=address

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

NAME:= webserv
# $(SRCS_DIR)/http/ $(SRCS_DIR)/server/
MODULES:= parser/
#$(SRCS_DIR)/http/ $(SRCS_DIR)/server/

INCLUDES:= -I./includes/ $(patsubst %,-I%,$(MODULES))

include $(patsubst %,%/module.mk,$(MODULES))
SRCS += $(addprefix $(MODULES), $(SRCS_LIST))
SRCS += main.cpp
# SRCS += $(addprefix $(SRCS_DIR)/, $(SRCS_LIST))

# OBJS_LIST:= $(patsubst %.cpp,%.o,$(SRCS))
OBJS:= $(patsubst %.cpp,%.o,$(SRCS))
# OBJS:= $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(OBJS_LIST))

# @echo "SOURCES LIST: " $(SRCS_LIST)
test:
	@echo "SOURCES: " $(SRCS)
	@echo "OBJECTS: " $(OBJS)

all: $(NAME)

run: re
	./$(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@
	@echo "$(YELLOW)[ EXECUTABLE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

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

re: fclean all

.PHONY: clean fclean all re debug run
