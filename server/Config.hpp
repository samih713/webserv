#include "webserv.hpp"
#include "../http/Message.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

using webserv::http::STATUS_CODE;

struct Location {
    string path;
    string modifier;
    string root;
    vector<string> indexFiles;
    bool autoindex;
    Location() : autoindex(false) {}
};

struct ErrorPage {
    STATUS_CODE code;
    string page;
};

struct ServerConfig {
    fd listenerPort;
    string maxBodySize;
    vector<string> serverName;
    string serverRoot;
    vector<string> indexFiles;
    vector<Location> locations;
    vector<ErrorPage> errorPages;
    bool autoindex;

    ServerConfig() : listenerPort(8080), autoindex(false) {}

    void print(void) const {
        cout << "ServerConfig {" << endl;
        cout << "  listenerPort: " << listenerPort << endl;
        cout << "  maxBodySize: " << maxBodySize << endl;
        cout << "  autoindex: " << autoindex << endl;
        cout << "  serverName: [";
        for (vector<string>::const_iterator itr = serverName.begin(); itr != serverName.end(); ++itr) {
            cout << *itr;
            if (itr + 1 != serverName.end())
                cout << ", ";
        }
        cout << "]" << endl;
        cout << "  serverRoot: " << serverRoot << endl;
        cout << "  locations: [" << endl;
        for (vector<Location>::const_iterator itr = locations.begin(); itr != locations.end(); ++itr) {
            cout << "    Location {" << endl;
            cout << "      path: " << itr->path << endl;
            cout << "      modifier: " << itr->modifier << endl;
            cout << "      root: " << itr->root << endl;
            cout << "      indexFiles: [";
            for (vector<string>::const_iterator itr2 = itr->indexFiles.begin(); itr2 != itr->indexFiles.end(); ++itr2) {
                cout << *itr2;
                if (itr2 + 1 != itr->indexFiles.end())
                    cout << ", ";
            }
            cout << "]" << endl;
            cout << "      autoindex: " << itr->autoindex << endl;
            cout << "    }" << endl;
        }
        cout << "  ]" << endl;
        cout << "  errorPages: [" << endl;
        for (vector<ErrorPage>::const_iterator itr = errorPages.begin(); itr != errorPages.end(); ++itr) {
            cout << "    ErrorPage {" << endl;
            cout << "      code: " << itr->code << endl;
            cout << "      page: " << itr->page << endl;
            cout << "    }" << endl;
        }
        cout << "  ]" << endl;
        cout << "}" << endl;
    }
};

#endif // CONFIG_HPP
