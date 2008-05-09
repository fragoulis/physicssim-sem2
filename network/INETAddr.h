#pragma once

#include <winsock2.h>
#include <string>
using namespace std;

class INETAddr
{
private:
	sockaddr_in _addr;

public:
	INETAddr(u_long IPaddr, u_short port);
	INETAddr(const char *hostName, const char *portNumber);
	INETAddr();

	u_short port() const { return ntohs (_addr.sin_port); }
	u_long IPaddr() const { return ntohl (_addr.sin_addr.s_addr); }

	int size() const { return sizeof(_addr); }
	sockaddr* addr() const { return (sockaddr *) &_addr; }

	string toString() const;
};
