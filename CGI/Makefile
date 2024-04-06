# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/23 12:40:53 by hmohamed          #+#    #+#              #
#    Updated: 2024/04/06 04:23:36 by hmohamed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME = temp
# INCLUDES:= -I./ -I../includes/ -Irequest/ -Iresponse/
# SRC = tester/main.cpp    	    \
# 		CGI_handler/Cgi.cpp

# OBJ = $(SRC:.cpp=.o)
# CXX = c++
# CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

# all: $(NAME)

# $(NAME) : $(OBJ)
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJ) -o $(NAME) 

# clean:
# 		rm -f $(OBJ)

# fclean: clean
# 		rm -f $(NAME)

# re: fclean all




include ../.common.mk

NAME:= libcgi.a

INCLUDES:= -I./ -I../includes/ -I../sources/HTTP/ -I../sources/HTTP/request/ -I../sources/HTTP/response/ -ICGI_handler 

libhttp = ../sources/HTTP/libhttp.a

# CGI
CGI_DIR:= CGI_handler
CGI_LIST:= Cgi.cpp 
CGI_SRCS:= $(addprefix $(CGI_DIR)/, $(CGI_LIST))

SRCS:=  $(CGI_SRCS)

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TEST_DIR:= tester
TEST_CGI:= $(TEST_DIR)/main.cpp

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

run: test_cgi
	./test_cgi

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready.\n"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR) $(OBJS_DIR)/CGI_handler

debug: CXXFLAGS += $(DEBUGFLAGS) $(SANITIZE)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

sanitize: CXXFLAGS += $(DEBUGFLAGS) $(SANITIZE)
sanitize: all
	@echo "$(MAGENTA)[ SANITIZE ]$(RESET) $(NAME) is ready for sanitizing."

valgrind: CXXFLAGS += $(DEBUGFLAGS)
valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_http
	@echo "$(MAGENTA)[ LEAKS ]$(RESET) Checking $(NAME) for leaks."

test_cgi: fclean debug
	@make -C ../sources/HTTP
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SANITIZE) $(INCLUDES) $(libhttp) $(TEST_CGI) $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) CGI is now ready for testing."


clean:
	@$(RM) $(OBJS_DIR) *.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	make fclean -C ../sources/HTTP
	@$(RM) $(NAME)
	@$(RM) test_cgi
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_cgi tests