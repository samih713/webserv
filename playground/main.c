#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
// error debugging
#define write_error_msg(message, error_code)            \
	do                                                  \
	{                                                   \
		write(STDERR_FILENO, message, strlen(message)); \
		write(STDERR_FILENO, "\n", 1);                  \
		return (error_code);                            \
	} while (false)

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
// communication through this socket is done, via specialised send() and recv() [read/write equivalents]
// they offer much greater ** control ** over data transmission

// @params
// Domain specifies a communications domain; this selects the protocol family which should be used.
// 	PF_LOCAL        Host-internal protocols, formerly called PF_UNIX,
// 	PF_UNIX         Host-internal protocols, deprecated, use PF_LOCAL,
// 	PF_INET         Internet version 4 protocols,
// 	PF_ROUTE        Internal Routing protocol,
// 	PF_KEY          Internal key-management function,
// 	PF_INET6        Internet version 6 protocols,
// 	PF_SYSTEM       System domain,
// 	PF_NDRV         Raw access to network device//

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
//  placed in non-blocking I/O mode, using fcntl.
// The select(2) call may be used to determine when it is possible to send more data.

/* ---------------------------------- recv --------------------------------- */
// ssize_t recv(int socket, void *buffer, size_t length, int flags);
//
// All three routines return the length of the message on successful completion.
// If a message is too long to fit in the supplied buffer, excess bytes may be discarded depending on the type of socket the message is received from.
// If no messages are available at the socket, the receive call waits for a message to arrive, unless the socket is nonblocking (see fcntl(2)) in which case the value -1 is returned and the external variable errno set to EAGAIN.
// The receive calls normally return any data available, up to the requested amount, rather than waiting for receipt of the full amount requested; this behavior is affected by the socket-level options
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
// this struct is used to prep socket address structres for subsequent use.

#if 0  // addrinfo
struct addrinfo {
	int	ai_flags;	/* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
	int	ai_family;	/* PF_xxx */
	int	ai_socktype;	/* SOCK_xxx */
	int	ai_protocol;	/* 0 or IPPROTO_xxx for IPv4 and IPv6 */
	socklen_t ai_addrlen;	/* length of ai_addr */
	char	*ai_canonname;	/* canonical name for hostname */
	struct	sockaddr *ai_addr;	/* binary address */
	struct	addrinfo *ai_next;	/* next structure in linked list */
};
// #else // just notes
struct addrinfo
{
	int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
	int ai_family; // this can be IPv4 or IPv6 or just AF_UNSPEC
	// ...
	struct sockaddr *ai_addr;
};
#endif // addrinfo

#if 0
struct sockaddr {
	__uint8_t       sa_len;         /* total length */
	sa_family_t     sa_family;      /* [XSI] address family */
	char            sa_data[14];    /* [XSI] addr value (actually larger) */
};
#endif // sockaddr
// To deal with the sa_data field, theres a parallel struct called sockaddr_in which
// can be passed to connect eventhough it requires a sockaddr
/*
 * Socket address, internet style.
 */
#if 0
struct sockaddr_in {
	__uint8_t       sin_len;
	sa_family_t     sin_family;
	in_port_t       sin_port; // must be in NBO (htons)
	struct  in_addr sin_addr;
	char            sin_zero[8]; // there to pad into the same size, set to all 0s
};
#endif // sockaddr_in
// this struct makes it easy to reference elements of the socket addr
#if 0
/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
	in_addr_t s_addr;
};
#endif // in_addr
// sometimes you don't know if a function call will fill out the sockaddr struct with a IPv4 or IPv6
// use the below struct and then check the family or length, and then typecast it to the one needed
#if 0
/*
 * [XSI] sockaddr_storage
 */
struct sockaddr_storage {
	__uint8_t       ss_len;         /* address length */
	sa_family_t     ss_family;      /* [XSI] address family */
	char                    __ss_pad1[_SS_PAD1SIZE];
	__int64_t       __ss_align;     /* force structure storage alignment */
	char                    __ss_pad2[_SS_PAD2SIZE];
};
#endif // storage

#include <arpa/inet.h> // htonl, inet_pton()

/* ------------------------------------------------------------------------- */
/*                     IP Addressing functions (in_addr)                     */
/* ------------------------------------------------------------------------- */

// lets say you have an address IP "10.12.110.57" that you want to store in a sockaddr_in,
// use int inet_pton(int af, const char * restrict src, void * restrict dst)
// it converts a presentation format address (standard "." notation) to network format usually a struct in_addr in NBO
// returns 1 if the address was valid for the sepcified family af, 0 if address wasn't parsable, -1 if some system error
// valid for AF_INET and AF_INET6 (IPv4 and IPv6)

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#if 0  // testing inet_pton and inet_ntop functions
// these functions only work with numeric IP addresses, no DNS lookup (no www.pornhub.com), you use getaddrinfo() for that
int main(void)
{
	struct sockaddr_in sa;	 // IPv4
	struct sockaddr_in6 sa6; // IPv6

	if (inet_pton(AF_INET, "216.58.204.142", &(sa.sin_addr)) < 1) // converts from "." notation to in_addr struct
		write_error_msg("inet_pton failed", -1);
	char *backToStandardNotation = malloc(INET_ADDRSTRLEN);
	if (!backToStandardNotation)
		write_error_msg("malloc failed my guy", -2);
	if (!inet_ntop(AF_INET, &sa.sin_addr, backToStandardNotation, INET_ADDRSTRLEN)) // convert back to standard notation
		write_error_msg(strerror(errno), -1);
	printf("%25s\n", backToStandardNotation);
}
#endif //

/* ------------------------------------------------------------------------- */
/*                    system calls and what is their order                   */
/* ------------------------------------------------------------------------- */

// [step 1]: fucking call getaddrinfo()
// helps setup the structs you need later on, does DNS lookup and fills up structs

#if 1
int main(void)
{
	const char *LAIN = "http://lain.angelic-trust.net/wired.html";
	const char *service = "http"; // can be the name of a particular service (ftp, telnet, ... etc) or a port number
	const struct addrinfo *hints = NULL; // a struct of addrinfo that you've already filled out with relevant information
	struct addrinfo *result; // fill this shit up, this is the head of a linked list of addrinfo's

	int status;
	if ((status = getaddrinfo(LAIN, service, hints, &result)) != 0)
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
}

#endif //
