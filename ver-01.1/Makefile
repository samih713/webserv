CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98

RM:= rm -rf

INCLUDES:= -I./includes

SRCS_DIR:= srcs/
SRCS:= Server.cpp

OBJS_DIR:= objs
OBJS:= $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

NAME:= webserv

all: $(NAME)

run: re
	./$(NAME)

ndebug: CXXFLAGS += -DNDEBUG
ndebug: run

debug: CXXFLAGS += -DDEBUG -ggdb3
debug: run

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $@

$(OBJS_DIR):
	@mkdir -p objs

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean all re

