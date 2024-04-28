#include "Message.hpp"
#include "webserv.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Location {
    string         uri;
    string         root;
    vector<string> indexFiles;
    bool           autoindex;
    size_t         maxBodySize;
    vector<string> methods;
    Location() : autoindex(false), maxBodySize(1000000) {}
};

struct ServerConfig {
    fd                       port;
    in_addr_t                host;
    string                   serverName;
    size_t                   maxBodySize;
    string                   root;
    vector<string>           indexFiles;
    bool                     autoindex;
    map<STATUS_CODE, string> errorPages;
    vector<Location>         locations;

    ServerConfig()
        : port(8080), host(htonl(INADDR_ANY)), maxBodySize(1000000), autoindex(false)
    {}

    void print(void) const
    {
        cout << "ServerConfig {" << endl;
        cout << "  port: " << port << endl;
        cout << "  host: " << inet_ntoa(*(struct in_addr*) &host) << endl;
        cout << "  maxBodySize: " << maxBodySize << endl;
        cout << "  indexFiles: [";
        for (vector<string>::const_iterator itr = indexFiles.begin();
             itr != indexFiles.end(); ++itr)
        {
            cout << *itr;
            if (itr + 1 != indexFiles.end())
                cout << ", ";
        }
        cout << "]" << endl;
        cout << "  autoindex: " << autoindex << endl;
        cout << "  serverName: [" << serverName << "]" << endl;
        cout << "  root: " << root << endl;
        cout << "  locations: [" << endl;
        for (vector<Location>::const_iterator itr = locations.begin();
             itr != locations.end(); ++itr)
        {
            cout << "    Location {" << endl;
            cout << "      path: " << itr->path << endl;
            cout << "      modifier: " << itr->modifier << endl;
            cout << "      root: " << itr->root << endl;
            cout << "      indexFiles: [";
            for (vector<string>::const_iterator itr2 = itr->indexFiles.begin();
                 itr2 != itr->indexFiles.end(); ++itr2)
            {
                cout << *itr2;
                if (itr2 + 1 != itr->indexFiles.end())
                    cout << ", ";
            }
            cout << "]" << endl;
            cout << "      autoindex: " << itr->autoindex << endl;
            cout << "      maxBodySize: " << itr->maxBodySize << endl;
            cout << "      methods: [";
            for (vector<string>::const_iterator itr2 = itr->methods.begin();
                 itr2 != itr->methods.end(); ++itr2)
            {
                cout << *itr2;
                if (itr2 + 1 != itr->methods.end())
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
