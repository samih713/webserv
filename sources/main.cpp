#include "ConfigParser.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"
#include "webserv.hpp"

void signal_handler(int signal)
{
    if (signal == SIGINT)
        g_stopServer = 1;
}

int main(int argc, char** argv)
{
    if (argc > 2) {
        cerr << "Usage: " << argv[0] << " [<config_file>]" << endl;
        exit(1);
    }

    signal(SIGINT, signal_handler);

    try {
        string configFile = (argc == 2) ? argv[1] : "./configs/webserv.conf";

        ConfigParser&        parser  = ConfigParser::get_instance(configFile);
        vector<ServerConfig> servers = parser.parse();
        LOG_INFO("Parsing " + configFile);

        Server& webserv = Server::get_instance(servers, 100);
        webserv.start(SELECT);
    } catch (std::exception& error) {
        LOG_ERROR(error.what());
    }
}
