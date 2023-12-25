#include <exception>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef _ADDRINFO_HPP_
#define _ADDRINFO_HPP_

#define SUCCESS 0
#define FAIL    !SUCCESS

class AddrInfo
{
private:
	int              _status;
	struct addrinfo *AddressInfo;

protected:

public:
	AddrInfo(const char *hostName         = nullptr, const char *service = nullptr,
	         const struct addrinfo *hints = nullptr)
	    : _status(0), AddressInfo{ nullptr }
	{
		_status = getaddrinfo(hostName, service, hints, &AddressInfo);
		if(_status == FAIL)
			throw BadAddress(_status);
	}

	~AddrInfo()
	{
		freeaddrinfo(AddressInfo);
	};

	/* ------------------------------- exception ------------------------------- */

	struct BadAddress : public std::exception
	{
	private:
		int _statusError;

	public:
		BadAddress(int status) : _statusError(status) {}
		const char *what() const noexcept override
		{
			return gai_strerror(_statusError);
		}
	};
};

#endif // _ADDRINFO_HPP_
