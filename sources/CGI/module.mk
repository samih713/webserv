# CGI makefile

CGI_SRCS:= $(CGI_DIR)/Cgi.cpp
CGI_OBJS:= $(CGI_SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS += $(CGI_OBJS)

# tester mains
TEST_CGI:= $(CGI_DIR)/test_cgi
TEST_CGI_SRC:= $(CGI_DIR)/$(TESTS_DIR)/test_cgi.cpp
