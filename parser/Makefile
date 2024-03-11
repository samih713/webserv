include ../common.mk

NAME:= libparser.a

INCLUDES:= -I./ -I../includes/

SRCS:= ConfigParser.cpp JsonParser.cpp

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TEST_DIR:= tester
TEST_PARSER:= $(TEST_DIR)/test_parser.cpp

CONFIG_DIR:= json
CONFIG_FILES:= $(CONFIG_DIR)/config_sample.json $(CONFIG_DIR)/simple.json

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready.\n"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p objects

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
	@$(RM) $(OBJS_DIR) *.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@$(RM) test_parser
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re test_parser tests
