# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/23 12:40:53 by hmohamed          #+#    #+#              #
#    Updated: 2024/03/06 15:18:29 by hmohamed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = temp
INCLUDES:= -I./ -I../includes/ -Irequest/ -Iresponse/
SRC = tester/main.cpp    	    \
		CGI_handler/Cgi.cpp

OBJ = $(SRC:.cpp=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all: $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJ) -o $(NAME) 

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all