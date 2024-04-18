#include "webserv.hpp"
#include "Message.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Location {
    string         path;
    string         modifier;
    string         root;
    vector<string> indexFiles;
    bool           autoindex;
    Location() : autoindex(false) {}
};

struct ServerConfig {
    fd                       listenerPort;
    string                   maxBodySize;
    string                   serverName;
    string                   serverRoot;
    vector<string>           indexFiles;
    vector<Location>         locations;
    map<STATUS_CODE, string> errorPages;
    bool                     autoindex;

    ServerConfig() : listenerPort(8080), autoindex(false) {}

    void print(void) const
    {
        cout << "ServerConfig {" << endl;
        cout << "  listenerPort: " << listenerPort << endl;
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
