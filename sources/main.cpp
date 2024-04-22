#include "Server.hpp"
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"
#include "webserv.hpp"

int main(int argc, char **argv) {
    if (argc > 2)
    {
        cerr << "Usage: " << argv[0] << " [<config_file>]" << endl;
        exit(1);
    }

    try
    {
        string config_file = (argc == 2) ? argv[1] : "./configs/default.conf";
        ConfigParser parser(config_file);
        vector<ServerConfig> configs = parser.parse();
        // configs[0].print();
        Server &webserv = Server::get_instance(configs[0], 10);
        webserv.start(KQUEUE);
    }
    catch (std::exception &error)
    {
        cerr << error.what() << endl;
    }
}
