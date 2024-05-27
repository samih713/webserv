#include "Message.hpp"
#include "webserv.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Location {
    string         root;
    string         indexFile;
    bool           autoindex;
    size_t         maxBodySize;
    vector<string> methods;
    Location() : indexFile("index.html"), autoindex(false), maxBodySize(1000000) {}
};

struct ServerConfig {
    fd                       port;
    in_addr_t                host;
    bool                     defaultServer;
    string                   serverName;
    string                   root;
    string                   indexFile;
    bool                     autoindex;
    size_t                   maxBodySize;
    map<STATUS_CODE, string> errorPages; //! maybe need default values
    map<string, Location>    locations;

    ServerConfig()
        : port(8080), host(htonl(INADDR_ANY)), defaultServer(false),
          indexFile("index.html"), autoindex(false), maxBodySize(1000000)
    {}

    void print(void) const
    {
        cout << "ServerConfig {" << endl;
        cout << "  port: " << port << endl;
        cout << "  host: " << inet_ntoa(*(struct in_addr*) &host) << endl;
        cout << "  default_server: " << (defaultServer == true ? "yes" : "no") << endl;
        cout << "  serverName: [" << serverName << "]" << endl;
        cout << "  root: " << root << endl;
        cout << "  indexFile: " << indexFile << endl;
        cout << "  autoindex: " << (autoindex == true ? "yes" : "no") << endl;
        cout << "  maxBodySize: " << maxBodySize << endl;
        cout << "  locations: [" << endl;
        for (map<string, Location>::const_iterator itr = locations.begin();
             itr != locations.end(); ++itr)
        {
            cout << "    Location {" << endl;
            cout << "      uri: " << itr->first << endl;
            cout << "      root: " << itr->second.root << endl;
            cout << "      indexFile: " << itr->second.indexFile << endl;
            cout << "      autoindex: " << (itr->second.autoindex == true ? "yes" : "no")
                 << endl;
            cout << "      maxBodySize: " << itr->second.maxBodySize << endl;
            cout << "      methods: [";
            for (vector<string>::const_iterator itr2 = itr->second.methods.begin();
                 itr2 != itr->second.methods.end(); ++itr2)
            {
                cout << *itr2;
                if (itr2 + 1 != itr->second.methods.end())
                    cout << ", ";
            }
            cout << "]" << endl;
            cout << "    }" << endl;
        }
        cout << "  ]" << endl;
        cout << "  errorPages: [" << endl;
        for (map<STATUS_CODE, string>::const_iterator itr = errorPages.begin();
             itr != errorPages.end(); ++itr)
        {
            cout << "    " << itr->first << ": " << itr->second << endl;
        }
        cout << "  ]" << endl;
        cout << "}" << endl;
    }
};

#endif // CONFIG_HPP