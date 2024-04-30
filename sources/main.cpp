#include "ConfigParser.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"
#include "webserv.hpp"

int main(int argc, char** argv)
{
    if (argc > 2) {
        cerr << "Usage: " << argv[0] << " [<config_file>]" << endl;
        exit(1);
    }

    try {
        string configFile = (argc == 2) ? argv[1] : "./configs/default.conf";

        ConfigParser&        parser  = ConfigParser::get_instance(configFile);
        vector<ServerConfig> configs = parser.parse();
        Logger::log_message("Parsing " + configFile, INFO);

        // configs[0].print();
        Server& webserv = Server::get_instance(configs[0], 10);
#if defined(__LINUX__)
        webserv.start(SELECT);
#elif defined(__MAC__)
        webserv.start(KQUEUE);
#endif
    } catch (std::exception& error) {
        Logger::log_message(error.what(), ERROR);
    }
}
