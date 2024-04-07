# server makefile

SRCS += $(SERVER_DIR)/socket/Socket.cpp $(SERVER_DIR)/socket/TCPSocket.cpp $(SERVER_DIR)/connection_manager/ConnectionManager.cpp $(SERVER_DIR)/Server.cpp

# tester mains
TEST_SOCKET:= $(TESTS_DIR)/SOCKET_main.cpp
