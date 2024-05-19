# http makefile

REQUEST_DIR:=$(HTTP_DIR)/request
REQUEST_SRCS:= $(wildcard $(REQUEST_DIR)/*.cpp) $(wildcard $(REQUEST_DIR)/header/*.cpp)

RESPONSE_DIR:=$(HTTP_DIR)/response
RESPONSE_SRCS:= $(wildcard $(RESPONSE_DIR)/*.cpp)

HANDLER_DIR:=$(HTTP_DIR)/handler
HANDLER_SRCS:= $(wildcard $(HANDLER_DIR)/*.cpp)

SRCS+= $(REQUEST_SRCS) $(RESPONSE_SRCS) $(HANDLER_SRCS)

# tester mains
TEST_HTTP:= $(HTTP_DIR)/test_http
TEST_HTTP_SRC:= $(HTTP_DIR)/$(TESTS_DIR)/test_http.cpp
