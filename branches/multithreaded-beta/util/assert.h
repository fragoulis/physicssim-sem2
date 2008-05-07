#pragma once
#include <cassert>

#ifdef _DEBUG
#define massert( expression, message ) assert( (expression) && message )
#define fassert( message )             massert( 0, message )
#else
#define massert( expression, message )
#define fassert( message )
#endif