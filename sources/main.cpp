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
        string configFile = (argc == 2) ? argv[1] : "./configs/webserv.conf";

        ConfigParser&        parser  = ConfigParser::get_instance(configFile);
        vector<ServerConfig> servers = parser.parse();
		LOG_INFO("Parsing " + configFile);

        // for (size_t i = 0; i < servers.size(); i++)
        //     servers[i].print();
        Server& webserv = Server::get_instance(servers, 100);
#if defined(__LINUX__)
        webserv.start(SELECT);
#elif defined(__MAC__)
        webserv.start(SELECT);
#endif
    } catch (std::exception& error) {
		LOG_ERROR(error.what());
    }
}
