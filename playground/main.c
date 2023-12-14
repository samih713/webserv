// sockets are a way to speak to other programs using standard unix ** file descriptors **
// When Unix programs do any sort of I/O they do by reading or writing to a file descriptor,
// fd's can be network connection also.

// A ** socket ** is a file descriptor for network communication
#include <sys/socket.h>

/* ------------------------------------------------------------------------- */
/*                                  sockets                                  */
/* ------------------------------------------------------------------------- */

// @breif creates an endpoint for communication and returns a descriptor
//	int socket(int domain, int type, int protocol);
// communication through this socket is done, via specialised send() and recv() [read/write equivalents i guess]
// they offer much greater ** control ** over data transmission

// Domain specifies a communications domain; this selects the protocol family which should be used.
// 	PF_LOCAL        Host-internal protocols, formerly called PF_UNIX,
// 	PF_UNIX         Host-internal protocols, deprecated, use PF_LOCAL,
// 	PF_INET         Internet version 4 protocols,
// 	PF_ROUTE        Internal Routing protocol,
// 	PF_KEY          Internal key-management function,
// 	PF_INET6        Internet version 6 protocols,
// 	PF_SYSTEM       System domain,
// 	PF_NDRV         Raw access to network device// @params

// Type, specifies the semantics of communication.
//  SOCK_STREAM type provides sequenced, reliable, two-way connection based byte streams.
//  SOCK_DGRAM socket supports datagrams (connectionless, unreliable messages of a fixed (typically small) maximum length).
//  SOCK_RAW sockets provide access to internal network protocols and interfaces, type SOCK_RAW, is available only to the super-user.

// Protocol, specifies a praticular protocol to be used with the socket

/* ---------------------------------- send --------------------------------- */
// @breif
// ssize_t send(int socket, const void *buffer, size_t length, int flags);
//
// If no messages space is available at the socket to hold the message to be transmitted, then send() normally blocks, unless the socket has been
//  placed in non-blocking I/O mode.  The select(2) call may be used to determine when it is possible to send more data.

/* ---------------------------------- recv --------------------------------- */
// ssize_t recv(int socket, void *buffer, size_t length, int flags);
//
// All three routines return the length of the message on successful completion.  If a message is too long to fit in the supplied buffer, excess
// bytes may be discarded depending on the type of socket the message is received from. If no messages are available at the socket, the receive call waits for a message to arrive, unless the socket is nonblocking (see fcntl(2)) in which case the value -1 is returned and the external variable errno set to EAGAIN.
// The receive calls normally return any data available, up to
// the requested amount, rather than waiting for receipt of the full amount requested; this behavior is affected by the socket-level options
// SO_RCVLOWAT and SO_RCVTIMEO described in getsockopt(2).

/* -------------------------------- sockets -------------------------------- */
// 2 types of internet sockets, while there are more but 2 are important to the discussion
// stream sockets and datagram sockets ** (SOCK_STREAM) (SOCK_DGRAM) **
// STREAM sockets are reliable two way connected communication streams. error free send 1,2 arrive 1,2 ... etc
// STREAM sockets follow TCP to achieve this high level of data quality, TCP ensures that data arrives sequentially and error free

// (DGRAM) sockets are conectionless sockets, when a datagram is sent

/* ------------------------------------------------------------------------- */
/*                                   Ports                                   */
/* ------------------------------------------------------------------------- */

// when a packet is created, it is wrapped in protocols.
/* -------------------------------- example -------------------------------- */
// [[packet] TFTP] the packet is encapsulated in a header by the first protocol here for example Trivial File Transfer Protocol
// [[[[[packet] TFTP] UDP/TCP] IP] --> physical protocols packet gets wrapped by alot of protocols
// when a packet is recieved the packet is stripped from its headers, kernel strips IP and UDP/TCP, then the program strips TFTP ...
// from a socket programming perspective the network hardware and topology is transparent

// [IP layer] -----		 [Host to Host transfer layer TCP/UDP]
// IP address -----   Port number:16 bit local connection address
// ports are necessary to distinguish between the different services that need to be handled on a single machine (web and mail ... etc)

/* ------------------------------------------------------------------------- */
/*                                   Endian                                  */
/* ------------------------------------------------------------------------- */
// 2 methods to store data depending on the processor architicutre, little and big endian
// Computer (Host) stores in Host Byte Order, and Network Byte Order this is always Big Endian
// The host could be little endian or big endian, its not guaranteed. Since this is not guaranteed
// there are functions that performs the conversion from HBO in case it was little endian, to NBO big endian
// htons() host to network short, there are similar funcitons for other types and in reverse ** man htons **
// Convert to network before sending, and convert back to host upon recieving

/* ------------------------------------------------------------------------- */
/*                               socket struct                               */
/* ------------------------------------------------------------------------- */
// the socket function returns an int, a socket descriptor
//

struct addrinfo
{
	int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
};

int main(void)
{

}
