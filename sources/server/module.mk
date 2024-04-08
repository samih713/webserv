# server makefile

SERVER_SRCS:= $(SERVER_DIR)/socket/Socket.cpp $(SERVER_DIR)/socket/TCPSocket.cpp $(SERVER_DIR)/connection_manager/ConnectionManager.cpp $(SERVER_DIR)/Server.cpp
SERVER_OBJS:= $(SERVER_SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS += $(SERVER_OBJS)

# tester mains
TEST_SOCKET:= $(SERVER_DIR)/test_socket
TEST_SOCKET_SRC:= $(SERVER_DIR)/$(TESTS_DIR)/SOCKET_main.cpp
TEST_CLIENT_SRC:= $(SERVER_DIR)/$(TESTS_DIR)/ClientTest.cpp

