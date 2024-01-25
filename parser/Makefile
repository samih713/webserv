###	COLORS ####
BLUE:= \033[1;34m
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98 -MMD -MP
DEBUGFLAGS:= -ggdb3 -D__DEBUG__ -fsanitize=address

ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

RM:= rm -rf

INCLUDES:= -I./ -I../includes/

SRCS:= ConfigParser.cpp

OBJS_DIR:= objects
OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

# tester mains
TEST_DIR:= tester
TEST_PARSER:= $(TEST_DIR)/test_parser.cpp

DEP:= $(OBJS:%.o=%.d)

CONFIG_FILES:= config_sample.json simple.json

NAME:= libparser.a

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

valgrind: CXXFLAGS += $(DEBUGFLAGS)
valgrind: CXXFLAGS -= -fsanitize=address
valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_parser $(CONFIG_FILES)
	@echo "$(MAGENTA)[ LEAKS ]$(RESET) Checking $(NAME) for leaks."

test_parser: $(TEST_PARSER) debug
	@$(CXX) $(CXXFLAGS) -fsanitize=address $(INCLUDES) $(TEST_PARSER) $(NAME) -o $@
	@mv *.d objects/
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
