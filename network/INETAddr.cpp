#include "inetaddr.h"
#include <sstream>
#include <iostream>
using namespace std;


INETAddr::INETAddr() {
	// Clear socket structure
	memset (&_addr, 0, sizeof(_addr));
}

INETAddr::INETAddr(u_long IPaddr, u_short port) {
	// Clear socket structure
	memset (&_addr, 0, sizeof(_addr));

	// Set socket to Internet Domain
	_addr.sin_family = AF_INET;

	// Set socket input port as port
	_addr.sin_port = htons( port );

	// Set the socket input IP as addr
	_addr.sin_addr.s_addr = htonl( IPaddr );
}

INETAddr::INETAddr(const char *hostName, const char *portNumber) {
	// Clear socket structure
	memset (&_addr, 0, sizeof(_addr));

	// Set socket to Internet Domain
	_addr.sin_family = AF_INET;

	if ( hostName != NULL ) {
		// Try address as IP of the form xxx.xxx.xxx.xxx
		u_long ip = inet_addr( hostName );
		if ( ip != INADDR_NONE ) {
			// Set socket input IP as the host
			_addr.sin_addr.S_un.S_addr = ip;
		} else {
			// Try to locate host by name
			hostent *host = gethostbyname( hostName );
			if ( host == NULL )
				// Set the socket input IP as any host
				_addr.sin_addr.s_addr = htonl( INADDR_ANY );

			// Set socket input IP as the host
			_addr.sin_addr = *(in_addr*)host->h_addr_list[0];
		}
	} else {
		// Set the socket input IP as any host
		_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	}

	// Convert port number from ascii to short
	char *endptr;
	u_short port = (u_short)strtol( portNumber, &endptr, 0 );
	if ( *endptr == '\0' ) {
		// Set socket input port as port
		_addr.sin_port = htons( port );
	} else {
		// Try and get the port from the service
		servent *sp = getservbyname( portNumber, "tcp" );
		if ( sp == NULL ) {
			_addr.sin_port = 0;
		} else {
			// Set socket input port as port
			_addr.sin_port = sp->s_port;
		}
	}
}

string INETAddr::toString() const {
	ostringstream sout;
	sout << inet_ntoa(_addr.sin_addr) << ':' << port();
	return sout.str();
}
