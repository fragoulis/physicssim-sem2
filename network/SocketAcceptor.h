#pragma once

#include "socketstream.h"

class SocketAcceptor : public Socket {
public:
	SocketAcceptor() {}
	SocketAcceptor(const INETAddr &localAddr) { open(localAddr); }

	bool open(const INETAddr &localAddr);
	bool accept(SocketStream&);
	bool listen();
};
