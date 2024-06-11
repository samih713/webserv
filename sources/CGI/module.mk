# CGI makefile

CGI_SRCS:= $(addprefix $(CGI_DIR)/, CGI.cpp)
SRCS += $(CGI_SRCS)

# tester mains
TEST_CGI:= $(CGI_DIR)/test_cgi
TEST_CGI_SRC:= $(CGI_DIR)/$(TESTS_DIR)/test_cgi.cpp
