# http makefile

# REQUEST
REQUEST_DIR:= request
REQUEST_LIST:= parse.cpp Request.cpp
REQUEST_SRCS:= $(addprefix $(REQUEST_DIR)/, $(REQUEST_LIST))
# RESPONSE
RESPONSE_DIR:= response
RESPONSE_LIST:= Response.cpp
RESPONSE_SRCS:= $(addprefix $(RESPONSE_DIR)/, $(RESPONSE_LIST))
# HANDLER
HANDLER_DIR:= handler
HANDLER_LIST:= GetRequestHandler.cpp
HANDLER_SRCS:= $(addprefix $(HANDLER_DIR)/, $(HANDLER_LIST))

SRCS_LIST += parse.cpp Request.cpp Response.cpp GetRequestHandler.cpp
SRCS += $(REQUEST_SRCS) $(RESPONSE_SRCS) $(HANDLER_SRCS)

# tester mains
TEST_HTTP:= $(TESTS_DIR)/main.cpp
