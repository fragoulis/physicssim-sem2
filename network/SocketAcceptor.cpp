#include "socketacceptor.h"
#include <fstream>
using namespace std;

extern ofstream dout;

bool SocketAcceptor::open(const INETAddr &localAddr) {
	// Create Internet Domain socket
	setHandle( ::socket( AF_INET, SOCK_STREAM, 0 ) );

	if (!isValid()) {
		dout << "Listening socket creation failed (code " << WSAGetLastError() << ")" <<endl;
		return false;
	}

	// Bind local address to the socket
	if ( bind( handle(), localAddr.addr(), localAddr.size() ) == SOCKET_ERROR ) {
		dout << "Bind to local port failed (code " << WSAGetLastError() << ")" <<endl;
		return false;
	}

	dout << "Opening listening socket ( local " << getLocalAddr().toString() << 
		" ) ( foreign " << getForeignAddr().toString() << " )"  << endl;

	return true;
}

bool SocketAcceptor::accept(SocketStream& s) {
	if (isValid()) {
		INETAddr addr;
		int size = addr.size();
		s.setHandle( ::accept(handle(), addr.addr(), &size) );

		if (s.isValid()) {
			dout << "Opening streaming socket ( local " << s.getLocalAddr().toString() << 
				" ) ( foreign " << s.getForeignAddr().toString() << " )"  << endl;
			return true;
		} else {
			dout << "Invalid streaming socket ( local " << s.getLocalAddr().toString() << 
				" ) ( foreign " << s.getForeignAddr().toString() << " )"  << endl;
		}
	} else {
		dout << "Invalid listening socket ( local " << getLocalAddr().toString() << 
			" ) ( foreign " << getForeignAddr().toString() << " )"  << endl;
	}
	return false;
}

bool SocketAcceptor::listen() {
	dout << "Listening" << endl;
	if ( ::listen(handle(), 5) )
		return false;

	return true;
}
