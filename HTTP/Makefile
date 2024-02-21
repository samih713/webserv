###	COLORS ####
BLUE:= \033[1;34m
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98 -MMD -MP
DEBUGFLAGS:= -ggdb3 -D__DEBUG__ 
SANITIZE:= -fsanitize=address

ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

RM:= rm -rf

INCLUDES:= -I./ -I../includes/ -Irequest/ -Iresponse/

# REQUESTS
REQUEST_DIR:= request
REQUEST_LIST:= Request.cpp parse.cpp
REQUEST_SRCS:= $(addprefix $(REQUEST_DIR)/, $(REQUEST_LIST))
# RESPONSE
RESPONSE_DIR:= response
RESPONSE_LIST:= Response.cpp IRequestHandler.cpp
RESPONSE_SRCS:= $(addprefix $(RESPONSE_DIR)/, $(RESPONSE_LIST))

SRCS:=  $(REQUEST_SRCS) $(RESPONSE_SRCS)

OBJS_DIR:= objs
OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TEST_DIR:= tester
TEST_HTTP:= $(TEST_DIR)/main.cpp

DEP:= $(OBJS:%.o=%.d)

NAME:= libhttp.a

all: $(NAME)

run: test_http
	./test_http

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready.\n"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p objs objs/request objs/response

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

test_http: fclean debug
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_HTTP) $(NAME) -o $@
	@mv *.d objs/
	@echo "$(BLUE)[ TEST ]$(RESET) HTTP is now ready for testing."


clean:
	@$(RM) $(OBJS_DIR) *.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@$(RM) test_http
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_http tests
