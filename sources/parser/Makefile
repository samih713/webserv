# parser makefile

include ../../.common.mk

NAME:= libparser.a

INCLUDES:= -I./ -I../../includes/

SRCS:= ConfigParser.cpp

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TESTS_DIR:= .tests
TEST_PARSER:= $(TESTS_DIR)/test_parser.cpp

CONFIG_DIR:= config_files
CONFIG_FILES:= $(wildcard $(CONFIG_DIR)/*.conf) $(wildcard $(CONFIG_DIR)/brace_check/*.conf)

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

sanitize: CXXFLAGS += $(DEBUGFLAGS) $(SANITIZE)
sanitize: all
	@echo "$(MAGENTA)[ SANITIZE ]$(RESET) $(NAME) is ready for sanitizing."

valgrind: CXXFLAGS += $(DEBUGFLAGS)
valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_parser $(CONFIG_FILES)
	@echo "$(MAGENTA)[ LEAKS ]$(RESET) Checking $(NAME) for leaks."

test_parser: $(TEST_PARSER) debug
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_PARSER) $(NAME) -o $@
	@mv *.d $(OBJS_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Parser is now ready for testing."

tests: test_parser
	@for config_file in $(CONFIG_FILES); do \
		echo "\n$(BLUE)[ TEST ]$(RESET) Running ./test_parser $$config_file."; \
		./test_parser $$config_file; \
	done

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
	@if [ -f test_parser ]; then \
		$(RM) test_parser; \
		echo "$(RED)[ DELETE ]$(RESET) Removed test_parser."; \
	fi

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_parser tests
