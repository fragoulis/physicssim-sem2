#include "socketstream.h"
#include <fstream>
using namespace std;

extern ofstream dout;

bool SocketStream::open(const INETAddr &foreignAddr, unsigned short localPort) {
	// Create Internet Domain socket
	setHandle( ::socket( AF_INET, SOCK_STREAM, 0) );

	if (!isValid()) {
		dout << "Stream socket creation failed (code " << WSAGetLastError() << ")" <<endl;
		return false;
	}

	// Bind local address to the socket -- IP is ignored
	if (localPort) {
		INETAddr localAddr(0, localPort);
		if ( bind( handle(), localAddr.addr(), localAddr.size() ) == SOCKET_ERROR ) {
			dout << "Bind to local port failed (code " << WSAGetLastError() << ")" <<endl;
			return false;
		}
	}

	// Connect to port
	if ( connect ( handle(), foreignAddr.addr(), foreignAddr.size() ) == SOCKET_ERROR ) {
		dout << "Connect to foreign port failed (code " << WSAGetLastError() << ")" <<endl;
		return false;
	}
	
	dout << "Opening streaming socket ( local " << getLocalAddr().toString() << 
		" ) ( foreign " << getForeignAddr().toString() << " )" << endl;
	return true;
}

int SocketStream::recv(char *buffer, int bufferSize, int flags) { 
	int bytesRead = ::recv(handle(), buffer, bufferSize, flags);

	if (bytesRead == SOCKET_ERROR) {
		dout << "Receiving error (code " << WSAGetLastError() << ")" <<endl;
	} else {
		dout << "Received " << bytesRead << " bytes \"" << buffer << "\"" << endl;
	}
	return bytesRead;
}

int SocketStream::send(const char *buffer, int bufferSize, int flags) {
	int bytesSent = ::send(handle(), buffer, bufferSize, flags);

	if (bytesSent == SOCKET_ERROR) {
		dout << "Sending error (code " << WSAGetLastError() << ")" <<endl;
	} else {
		dout << "Sending " << bytesSent << " bytes \"" << buffer << "\"" << endl;
	}
	return bytesSent;
}

