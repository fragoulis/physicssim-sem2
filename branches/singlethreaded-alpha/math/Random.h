#ifndef __TLIB_MATH_RANDOM_H__
#define __TLIB_MATH_RANDOM_H__
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#define FZERO_E 1e-6

namespace tlib
{
namespace math
{

//! Returns a random integer between min and max
inline int RandInt( const int min, const int max )
{
    if( min == max ) return min;
    assert(((max-min)<RAND_MAX)&&(max>min) && "RandInt() assertion" );
    return ( ( rand()%(max-min) ) + min );
}

//! Returns a random float between 0 and 1 with three digits precision
inline float RandUnit()
{
    return float( ( rand()%RAND_MAX ) / (float)RAND_MAX );
}

//! Returns a random float between min and max
inline float RandFloat( const float min, const float max )
{
    assert(max>min);
    return ( ( max - min ) * RandUnit() + min );
}

//! Returns a random integer between -@range and @range
inline int RandInt( const int range )
{
    assert((range>0)&&((range/2)<RAND_MAX));
    return ( ( rand()%(range+range) ) - range );
}

//! Returns a random bool
inline bool RandBoolean()
{
    return (RandUnit()<0.5f)?false:true;
}

} // end namespace math
} // end namespace tlib

#endif // __TLIB_MATH_RANDOM_H__