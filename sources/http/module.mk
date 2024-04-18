# http makefile

HTTP_SRCS:= $(HTTP_DIR)/request/Request.cpp $(HTTP_DIR)/request/recv.cpp $(HTTP_DIR)/request/parse_request.cpp $(HTTP_DIR)/response/Response.cpp $(HTTP_DIR)/handler/GetRequestHandler.cpp
HTTP_OBJS:= $(HTTP_SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS += $(HTTP_OBJS)

# tester mains
TEST_HTTP:= $(HTTP_DIR)/test_http
TEST_HTTP_SRC:= $(HTTP_DIR)/$(TESTS_DIR)/test_http.cpp
