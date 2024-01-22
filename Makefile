###	COLORS ####
BLUE:= \033[1;34m
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra
DEBUGFLAGS:= -ggdb3 -fsanitize=address -D__DEBUG__

ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

RM:= rm -rf

INCLUDES:= -I./includes

SRCS:= main.cpp
OBJS_DIR:= objects
OBJS:= $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

LIBRARY_FLAGS:= -Lserver/ -lserver -Lparser/ -lparser

NAME:= webserv

all: $(NAME)

run: re
	./$(NAME)

$(NAME): parser server $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@ $(LIBRARY_FLAGS)
	@echo "$(GREEN)[ COMPILE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p objs

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

# @make -sC tester/ # need to add tests for parser and server
tests:
	@make tests -sC parser/
	@make tests -sC server/
	@echo "$(BLUE)[ TEST ]$(RESET) Ready for testing."

parser:
	@make -sC parser/

server:
	@make -sC server/

clean:
	@$(RM) $(OBJS_DIR) *.o
	@make clean -sC parser/ > /dev/null 2>&1
	@make clean -sC server/ > /dev/null 2>&1
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@make fclean -sC parser/ > /dev/null 2>&1
	@make fclean -sC server/ > /dev/null 2>&1
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and libraries."

re: fclean all

.PHONY: clean fclean all re tests server parser debug run
