#include "server/Server.hpp"
#include "parser/ConfigParser.hpp"
#include "server/Config.hpp"
#include "includes/webserv.hpp"

int main()
{
    // default constructor of config
    // Config(string serverRoot = "../resources/sample_pages/",
    // string home = "index.html",
    // string notFoundPage = "404.html",
    // fd     listenerPort = 8080)
    //
    try
    {
        ConfigParser parser("configs/webserv.conf");
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
