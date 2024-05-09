# http makefile

HTTP_SRCS := $(addprefix $(HTTP_DIR)/, request/parse_request.cpp request/recv.cpp request/Request.cpp response/Response.cpp handler/GetRequestHandler.cpp)
SRCS += $(HTTP_SRCS)

# tester mains
TEST_HTTP:= $(HTTP_DIR)/test_http
TEST_HTTP_SRC:= $(HTTP_DIR)/$(TESTS_DIR)/test_http.cpp
