#include "Message.hpp"
#include "webserv.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Location {
    string         path;
    string         modifier;
    string         root;
    vector<string> indexFiles;
    bool           autoindex;
    size_t         maxBodySize;
    Location() : autoindex(false), maxBodySize(1000000) {}
};

struct ServerConfig {
    fd                       listenerPort;
    size_t                   maxBodySize;
    string                   serverName;
    string                   serverRoot;
    vector<string>           indexFiles;
    vector<Location>         locations;
    map<STATUS_CODE, string> errorPages;
    bool                     autoindex;
    in_addr_t                serverAddr;

    ServerConfig()
        : listenerPort(8080), maxBodySize(1000000), autoindex(false),
          serverAddr(htonl(INADDR_ANY))
    {}

    void print(void) const
    {
        cout << "ServerConfig {" << endl;
        cout << "  listenerPort: " << listenerPort << endl;
        cout << "  host: " << inet_ntoa(*(struct in_addr*) &serverAddr) << endl;
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
        cout << "  serverRoot: " << serverRoot << endl;
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
