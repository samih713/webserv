#include "./server/Server.hpp"
#include "./parser/ConfigParser.hpp"
#include "./server/ServerConfig.hpp"
#include "../includes/webserv.hpp"

int main(int argc, char **argv) {
    if (argc > 2) {
        std::cerr << "Usage 1: " << argv[0] << std::endl;
        std::cerr << "Usage 2: " << argv[0] << " <config_file>" << std::endl;
        exit(1);
    }
    string config_file = "./configs/default.conf";
    if (argc == 2)
        config_file = argv[1];
    try
    {
        ConfigParser parser(config_file);
        vector<ServerConfig> configs = parser.parse();
        // configs[0].print();
        Server &webserv = Server::get_instance(configs[0], 10);
        webserv.start(SELECT);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
}
