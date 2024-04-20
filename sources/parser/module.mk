# parser makefile

PARSER_SRCS:= $(PARSER_DIR)/ConfigParser.cpp
PARSER_OBJS:= $(PARSER_SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS += $(PARSER_OBJS)

# tester mains
TEST_PARSER:= $(PARSER_DIR)/$(TESTS_DIR)/test_parser
TEST_PARSER_SRC:= $(PARSER_DIR)/$(TESTS_DIR)/test_parser.cpp
