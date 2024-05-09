#include "ConfigParser.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <config_file>" << endl;
        exit(1);
    }

    try {
        ConfigParser         parser         = ConfigParser::get_instance(argv[1]);
        vector<ServerConfig> configurations = parser.parse();
        for (vector<ServerConfig>::const_iterator itr = configurations.begin();
             itr != configurations.end(); ++itr)
            itr->print();
    } catch (std::exception& error) {
        cerr << error.what() << endl;
        return 1;
    }
    return 0;
}
