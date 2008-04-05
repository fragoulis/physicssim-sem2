#pragma once

namespace tlib
{
namespace math
{

inline bool IsOdd( const int &number )
{
    return bool( number & 0x1 );
}

} // end namespace math
} // end namespace tlib