#include "Server.hpp"

/* --------------------------------- KQUEUE --------------------------------- */
// default kqueue wait
static const int KQUEUEWAITTIME(5);
// default max events to wait on
static const int MAX_EVENTS(16);

#if defined(__LINUX__)
void Server::kqueue_strat()
{
    THROW_EXCEPTION_WITH_INFO("Kqueue does not work on Linux");
}
#elif defined(__MAC__)
void Server::kqueue_strat()
{
    //     const fd              listenerFD    = listener.get_fd();
    //     const struct timespec kqueueTimeOut = { .tv_sec = KQUEUEWAITTIME, .tv_nsec = 0
    //     };

    //     struct kevent changeList;            // list of events to monitor
    //     struct kevent eventList[MAX_EVENTS]; // list of events that have occurred

    //     // creating kqueue
    //     const int kq = kqueue();
    //     if (kq == -1)
    //         THROW_EXCEPTION_WITH_INFO(strerror(errno));

    //     // adding listenerFD to changeList to monitor for read events
    //     EV_SET(&changeList, listenerFD, EVFILT_READ, EV_ADD, 0, 0, 0);

    //     // adding changeList containing only listenerFD to kqueue
    //     if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //         close(kq);
    //         THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //     }

    //     int numEvents = 0;
    //     while (true) // event loop
    //     {
    //         DEBUG_MSG(WAIT_MESSAGE, L);

    //         numEvents = kevent(kq, NULL, 0, eventList, MAX_EVENTS, &kqueueTimeOut);
    //         if (numEvents == -1) {
    //             close(kq);
    //             THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //         }
    //         else if (numEvents == 0) // timeout
    //             continue;

    //         for (int i = 0; i < numEvents; i++) {
    //             if (eventList[i].ident == static_cast<uintptr_t>(listenerFD)) {
    //                 // add new connection to kqueue
    //                 fd newConnection = listener.accept();
    //                 EV_SET(&changeList, newConnection, EVFILT_READ, EV_ADD, 0, 0, 0);
    //                 if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //                     close(kq);
    //                     THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //                 }
    //             }
    //             else if (eventList[i].flags & EV_EOF) {
    //                 // remove connection from kqueue
    //                 EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE, 0,
    //                 0, 0); if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //                     close(kq);
    //                     THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //                 }
    //                 close(eventList[i].ident);
    //                 // this should be done in handle_connection
    //             }
    //             else if (eventList[i].flags & EVFILT_READ) {
    //                 DEBUG_MSG("reading from connection", M);
    //                 //! this is not a good fix at all and is very hard-cody
    //                 try {
    //                     Request req;
    //                     req.recv(eventList[i].ident);
    //                     if (!req.process(cfg))
    //                         continue;

    //                     IRequestHandler* handler  =
    //                     make_request_handler(req.get_method()); Response response =
    //                     handler->handle_request(req);
    //                     response.send_response(eventList[i].ident);
    //                     delete handler;
    //                 } catch (std::ios_base::failure& f) {
    //                     DEBUG_MSG(ERR_PARSE, R);
    //                 } catch (std::exception& error) {
    //                     EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE,
    //                     0, 0,
    //                         0);
    //                     if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //                         close(kq);
    //                         THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //                     }
    //                     DEBUG_MSG(error.what(), R);
    //                 }
    //             }
    //         }
    //     }

    // close(kq);
}
#endif
