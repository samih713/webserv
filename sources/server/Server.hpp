#include "CachedPages.hpp"
#include "ErrorHandler.hpp"
#include "DeleteRequestHandler.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "Logger.hpp"
#include "PostRequestHandler.hpp"
#include "Request.hpp"
#include "ServerConfig.hpp"
#include "TCPSocket.hpp"
#include "webserv.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP

enum polling_strat {
    KQUEUE,
    SELECT,
    POLL,
    EPOLL
};

typedef map<TCPSocket, ServerConfig> servers_t;

// wait message
static const string WAIT_MESSAGE("*** Server is now waiting for connections ***");
// default backLog
static const int DEFAULT_BACKLOG(16);
// default select wait
static const int SELECTWAITTIME(5);
// default kqueue wait
static const int KQUEUEWAITTIME(5);
// default max events to wait on
static const int MAX_EVENTS(16);

class Server {
public:
    static Server& get_instance(vector<ServerConfig>& cfgs, int backLog = DEFAULT_BACKLOG)
    {
        static Server instance(cfgs, backLog);
        return instance;
    }

    ~Server()
    {
        for (vector<ServerConfig>::iterator sc = cfgs.begin(); sc != cfgs.end(); sc++)
            delete sc->cp;
    }

    void start(enum polling_strat);

private:
    servers_t servers;
	vector<ServerConfig> &cfgs;

    Server(vector<ServerConfig>& cfgs, int backLog);

    // IRequestHandler* make_request_handler(const string& method, ServerConfig& cfg)
    IRequestHandler* make_request_handler(const Request& r, ServerConfig& cfg)
    {
		const string &method = r.get_method();
		if (r.get_status() != OK)
			return new ErrorHandler(cfg);
        else if (method == "GET")
            return new GetRequestHandler(cfg);
        else if (method == "POST")
            return new PostRequestHandler(cfg);
        else if (method == "DELETE")
            return new DeleteRequestHandler(cfg);
        else //! return 501 Not implemented
            THROW_EXCEPTION_WITH_INFO("Request Method not implemented\n");
    }

    void handle_connection(fd incoming, ServerConfig& cfg);

    /* polling strats */
    void select_strat();
    void kqueue_strat();
    // void poll_strat();
    // void epoll_strat();

    // deleted
    Server(const Server&);
    Server& operator=(const Server&);
};

#endif // SERVER_HPP
