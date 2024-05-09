# parser makefile

PARSER_SRCS:= $(addprefix $(PARSER_DIR)/, ConfigParser.cpp)
SRCS += $(PARSER_SRCS)

# tester mains
TEST_PARSER:= $(PARSER_DIR)/$(TESTS_DIR)/test_parser
TEST_PARSER_SRC:= $(PARSER_DIR)/$(TESTS_DIR)/test_parser.cpp
