###	COLORS ####
BLUE:= \033[1;34m
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

CXX:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98
DEBUGFLAGS:= -ggdb3 -fsanitize=address -D__DEBUG__

ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

RM:= rm -rf

INCLUDES:= -I./

SRCS:= ConfigParser.cpp

# tester mains
TEST_DIR:= tester
TEST_PARSER:= $(TEST_DIR)/test_parser.cpp

OBJS_DIR:= objects
OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))
OBJS_BASENAMES:= $(addprefix $(OBJS_DIR)/,$(notdir $(OBJS)))

CONFIG_FILES:= config_sample.conf

NAME:= libparser.a

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready.\n"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $(OBJS_DIR)/$(notdir $@)
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p objects

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

test_parser: $(TEST_PARSER) debug
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -fsanitize=address $(TEST_PARSER) $(NAME) -o $@
	@echo "$(BLUE)[ TEST ]$(RESET) Parser is now ready for testing."

tests: test_parser
	@echo ""

clean:
	@$(RM) $(OBJS_DIR)
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@$(RM) test_parser
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and other executables."

re: fclean all

.PHONY: clean fclean all re test_parser tests