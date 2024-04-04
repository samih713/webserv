# common makefile

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
