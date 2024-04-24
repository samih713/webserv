# http makefile

REQUEST_DIR:=$(HTTP_DIR)/request
REQUEST_SRCS:= $(wildcard $(REQUEST_DIR)/*.cpp)

HTTP_SRCS:= $(REQUEST_SRCS) $(HTTP_DIR)/response/Response.cpp $(HTTP_DIR)/handler/GetRequestHandler.cpp
HTTP_OBJS:= $(HTTP_SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS += $(HTTP_OBJS)

# tester mains
TEST_HTTP:= $(HTTP_DIR)/test_http
TEST_HTTP_SRC:= $(HTTP_DIR)/$(TESTS_DIR)/test_http.cpp
