# server makefile

# SOCKET
SOCKET_DIR:= socket
SOCKET_LIST:= Socket.cpp TCPSocket.cpp
SOCKET_SRCS:= $(addprefix $(SOCKET_DIR)/, $(SOCKET_LIST))
# CONNECTION_MANAGER
CONNECTION_MANAGER_DIR:= connection_manager
CONNECTION_MANAGER_LIST:= ConnectionManager.cpp
CONNECTION_MANAGER_SRCS:= $(addprefix $(CONNECTION_MANAGER_DIR)/, $(CONNECTION_MANAGER_LIST))

SRCS_LIST += Socket.cpp TCPSocket.cpp ConnectionManager.cpp Server.cpp

# tester mains
TEST_SOCKET:= $(TESTS_DIR)/SOCKET_main.cpp
