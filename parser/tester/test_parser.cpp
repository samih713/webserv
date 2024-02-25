#include "../ConfigParser.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        exit(1);
    }

    try {
        JsonParser parser(argv[1]);
        JsonValue json = parser.parseJSON();
    } catch (std::exception& error) {
        std::cerr << error.what() << std::endl;
        exit(1);
    }
    return 0;
}