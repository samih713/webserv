# main makefile

include common.mk

NAME:= webserv

INCLUDES:= -I./includes

SRCS:= main.cpp

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

LIBRARY_FLAGS:= -Lserver/ -lserver -Lparser/ -lparser -Lhttp/ -lhttp

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

run: re
	./$(NAME)

$(NAME): parser server http $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@ $(LIBRARY_FLAGS)
	@echo "$(YELLOW)[ EXECUTABLE ]$(RESET) $(NAME) is ready."

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

# @make -sC tester/ # need to add tests for parser and server
tests:
	@make tests -sC parser/
	@make tests -sC server/
	@make tests -sC http/
	@echo "$(BLUE)[ TEST ]$(RESET) Ready for testing."

parser:
	@make -sC parser/

server:
	@make -sC server/

http:
	@make -sC http/

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) $(OBJS_DIR); \
		echo "$(RED)[ DELETE ]$(RESET) Removed object files."; \
	fi
	@make clean -sC parser/ > /dev/null 2>&1
	@make clean -sC server/ > /dev/null 2>&1
	@make clean -sC http/ > /dev/null 2>&1

fclean: clean
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME)."; \
	fi
	@make fclean -sC parser/ > /dev/null 2>&1
	@make fclean -sC server/ > /dev/null 2>&1
	@make fclean -sC http/ > /dev/null 2>&1

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re tests server parser http debug run
