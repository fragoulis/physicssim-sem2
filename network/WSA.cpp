#include "wsa.h"

WSA::WSA() {
	// Create version identifier
	WORD wVersionRequested = MAKEWORD( 2, 0 );

	// Startup windows sockets
	_status = !WSAStartup( wVersionRequested, &_wsaData );
}

WSA::~WSA() {
	WSACleanup();
}
