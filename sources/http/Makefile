# http makefile

include ../../.common.mk

NAME:= libhttp.a

INCLUDES:= -I./ -I../includes/ -Irequest/ -Iresponse/ -Ihandler/ -I../server/ \
		   -I../server/socket/ -I../server/connection_manager/

# REQUESTS
REQUEST_DIR:= request
REQUEST_LIST:= parse.cpp Request.cpp
REQUEST_SRCS:= $(addprefix $(REQUEST_DIR)/, $(REQUEST_LIST))
# RESPONSE
RESPONSE_DIR:= response
RESPONSE_LIST:= Response.cpp
RESPONSE_SRCS:= $(addprefix $(RESPONSE_DIR)/, $(RESPONSE_LIST))
# RESPONSE
HANDLER_DIR:= handler
HANDLER_LIST:= GetRequestHandler.cpp
HANDLER_SRCS:= $(addprefix $(HANDLER_DIR)/, $(HANDLER_LIST))

SRCS:=  $(REQUEST_SRCS) $(RESPONSE_SRCS) $(HANDLER_SRCS)

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TESTS_DIR:= .tests
TEST_HTTP:= $(TESTS_DIR)/main.cpp

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

run: test_http
	./test_http

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $@ $(addprefix $(OBJS_DIR)/, request response handler)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(NAME)
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

sanitize: CXXFLAGS += $(DEBUGFLAGS) $(SANITIZE)
sanitize: all
	@echo "$(MAGENTA)[ SANITIZE ]$(RESET) $(NAME) is ready for sanitizing."

valgrind: CXXFLAGS += $(DEBUGFLAGS)
valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_http
	@echo "$(MAGENTA)[ LEAKS ]$(RESET) Checking $(NAME) for leaks."

test_http: debug
	@$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SANITIZE) $(INCLUDES) $(TEST_HTTP) $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) HTTP is now ready for testing."

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
	@$(RM) test_http
# @echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_http tests
