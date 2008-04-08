#pragma once
#include <cassert>

#ifdef _DEBUG
#define massert( expression, message ) assert( (expression) && message )
#else
#define massert( expression, message )
#endif