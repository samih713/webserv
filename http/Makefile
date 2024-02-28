include ../common.mk

INCLUDES:= -I./ -I../includes/ -Irequest/ -Iresponse/

# REQUESTS
REQUEST_DIR:= request
REQUEST_LIST:= parse.cpp Request.cpp 
REQUEST_SRCS:= $(addprefix $(REQUEST_DIR)/, $(REQUEST_LIST))
# RESPONSE
RESPONSE_DIR:= response
RESPONSE_LIST:= Response.cpp IRequestHandler.cpp
RESPONSE_SRCS:= $(addprefix $(RESPONSE_DIR)/, $(RESPONSE_LIST))

SRCS:=  $(REQUEST_SRCS) $(RESPONSE_SRCS)

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
	@mkdir -p $(OBJS_DIR) $(OBJS_DIR)/request $(OBJS_DIR)/response

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
	@mv *.d $(OBJS_DIR)
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
