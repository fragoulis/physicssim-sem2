#include "socket.h"
#include <fstream>
using namespace std;

extern ofstream dout;

void Socket::close() { 
	dout << "Closing socket ( local " << getLocalAddr().toString() << 
		" ) ( foreign " << getForeignAddr().toString() << " )"  << endl;
	
	// Not a graceful method of closing a socket as data may be lost
	shutdown(_handle, SD_SEND);
	closesocket(_handle); 
	_handle = INVALID_SOCKET;
}

void Socket::setHandle(SOCKET handle) { 
	// Ensures exisitng sockets are not left open when overwritten
	if (isValid())
		close();
	_handle = handle; 
}

INETAddr Socket::getLocalAddr() const {
	INETAddr addr;
	int size = addr.size();
	getsockname(_handle, addr.addr(), &size);
	return addr;
}

INETAddr Socket::getForeignAddr() const {
	INETAddr addr;
	int size = addr.size();
	getpeername(_handle, addr.addr(), &size);
	return addr;
}
