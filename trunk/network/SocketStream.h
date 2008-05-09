#pragma once

#include "socket.h"

class SocketStream : public Socket {
public:
	SocketStream() {}
	SocketStream(SOCKET handle) : Socket(handle) {}
	SocketStream(const INETAddr &foreignAddr, unsigned short localPort=0) { open (foreignAddr, localPort); }

	bool open(const INETAddr &foreignAddr, unsigned short localPort=0);

	int recv(char *buffer, int bufferSize, int flags=0);
	int send(const char *buffer, int bufferSize, int flags=0);
};
