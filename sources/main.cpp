#include "./server/Server.hpp"
#include "./parser/ConfigParser.hpp"
#include "./server/ServerConfig.hpp"
#include "../includes/webserv.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        exit(1);
    }
    try
    {
        ConfigParser parser(argv[1]);
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
