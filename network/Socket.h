#pragma once

#include <winsock2.h>
#include "INETAddr.h"

class Socket
{
private:
	SOCKET _handle;

	// Hidden to prevent shallow copying, which closes the socket
	Socket(const Socket&);
	Socket operator=(const Socket&);

protected:
	Socket() : _handle(INVALID_SOCKET) {}
	Socket(SOCKET handle) : _handle(handle) {}

public:
	virtual ~Socket() { close(); }

	void close();

	void setHandle(SOCKET handle);
	SOCKET handle() const { return _handle; }

	bool isValid() const { return _handle!=INVALID_SOCKET; }

	INETAddr getLocalAddr() const;
	INETAddr getForeignAddr() const;
};
