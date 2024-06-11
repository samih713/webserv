# server makefile

SERVER_SRCS:= $(addprefix $(SERVER_DIR)/, socket/TCPSocket.cpp connection_manager/ConnectionManager.cpp CachedPages.cpp Server.cpp)

SRCS += $(SERVER_SRCS)

# tester mains
TEST_SOCKET:= $(SERVER_DIR)/test_socket
TEST_SOCKET_SRC:= $(SERVER_DIR)/$(TESTS_DIR)/SOCKET_main.cpp
TEST_CLIENT_SRC:= $(SERVER_DIR)/$(TESTS_DIR)/ClientTest.cpp
