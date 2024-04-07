# http makefile

SRCS += $(HTTP_DIR)/request/parse.cpp $(HTTP_DIR)/request/Request.cpp $(HTTP_DIR)/response/Response.cpp $(HTTP_DIR)/handler/GetRequestHandler.cpp

# tester mains
TEST_HTTP:= $(TESTS_DIR)/main.cpp
