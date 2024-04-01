#include "../ConfigParser.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        exit(1);
    }

    try {
        ConfigParser parser(argv[1]);
        Config configurations = parser.parse();
        configurations.print();
    }
    catch (std::exception &error) {
        std::cerr << R << "[ ERROR ] " << RE << error.what() << std::endl;
        return 1;
    }
    return 0;
}