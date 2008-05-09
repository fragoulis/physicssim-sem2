#pragma once

#include <winsock2.h>

class WSA
{
public:
	WSA();
	~WSA();

	bool isOk() const { return _status; }

private:
	WSADATA _wsaData;
	bool _status;
};
