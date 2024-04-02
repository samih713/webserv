include common.mk

NAME:= webserv

INCLUDES:= -I./includes

SRCS:= main.cpp

OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

LIB_HTTP:= ./http/libhttp.a
LIB_SERVER:= ./server/libserver.a
LIB_PARSER:= ./parser/libparser.a

LIBRARY_FLAGS:= -Lserver/ -lserver -Lparser/ -lparser -Lhttp/ -lhttp

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

run: re
	./$(NAME)

$(NAME): $(LIB_HTTP) $(LIB_PARSER) $(LIB_SERVER) $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@ $(LIBRARY_FLAGS)
	@echo "$(YELLOW)[ EXECUTABLE ]$(RESET) $(NAME) is ready.\n"

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
	@+make tests -sC parser/
	@+make tests -sC server/
	@+make tests -sC http/
	@echo "$(BLUE)[ TEST ]$(RESET) Ready for testing."

$(LIB_HTTP):
	@+make -sC http/

$(LIB_PARSER):
	@+make -sC parser/

$(LIB_SERVER):
	@+make -sC server/

clean:
	@$(RM) $(OBJS_DIR) *.o
	@+make clean -sC parser/ > /dev/null 2>&1
	@+make clean -sC server/ > /dev/null 2>&1
	@+make clean -sC http/ > /dev/null 2>&1
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@+make fclean -sC parser/ > /dev/null 2>&1
	@+make fclean -sC server/ > /dev/null 2>&1
	@+make fclean -sC http/ > /dev/null 2>&1
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME) and libraries.\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re tests server parser http debug run
