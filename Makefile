# main makefile

include .common.mk

NAME:= webserv

INCLUDES:= -I./includes

SRCS_LIST:= main.cpp
SRCS:= $(addprefix $(SRCS_DIR)/, $(SRCS_LIST))

OBJS_LIST:= $(SRCS_LIST:%.cpp=%.o)
OBJS:= $(addprefix $(OBJS_DIR)/, $(OBJS_LIST))

HTTP_DIR:= $(SRCS_DIR)/http/
PARSER_DIR:= $(SRCS_DIR)/parser/
SERVER_DIR:= $(SRCS_DIR)/server/

LIBRARY_FLAGS:= -L$(SERVER_DIR) -lserver -L$(PARSER_DIR) -lparser -L$(HTTP_DIR) -lhttp

DEP:= $(OBJS:%.o=%.d)

all: libraries $(NAME)

run: re
	./$(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@ $(LIBRARY_FLAGS)
	@echo "$(YELLOW)[ EXECUTABLE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug:
	@make debug -sC $(HTTP_DIR)
	@make debug -sC $(PARSER_DIR)
	@make debug -sC $(SERVER_DIR)
	@make -sC ./
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

# @make -sC tester/ # need to add tests for parser and server
tests:
	@make tests -sC $(HTTP_DIR)
	@make tests -sC $(PARSER_DIR)
	@make tests -sC $(SERVER_DIR)
	@echo "$(BLUE)[ TEST ]$(RESET) Ready for testing."

libraries:
	@make -sC $(HTTP_DIR)
	@make -sC $(PARSER_DIR)
	@make -sC $(SERVER_DIR)

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) $(OBJS_DIR); \
		echo "$(RED)[ DELETE ]$(RESET) Removed object files."; \
	fi
	@make clean -sC $(HTTP_DIR) > /dev/null 2>&1
	@make clean -sC $(PARSER_DIR) > /dev/null 2>&1
	@make clean -sC $(SERVER_DIR) > /dev/null 2>&1

fclean: clean
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME)."; \
	fi
	@make fclean -sC $(HTTP_DIR) > /dev/null 2>&1
	@make fclean -sC $(PARSER_DIR) > /dev/null 2>&1
	@make fclean -sC $(SERVER_DIR) > /dev/null 2>&1

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re tests debug run
