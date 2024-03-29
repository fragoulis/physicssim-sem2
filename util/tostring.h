#pragma once
#include <string>
#include <sstream>
#include <stdexcept>

class BadConversion : public std::runtime_error { 
public:
    BadConversion( const std::string& s )
        : std::runtime_error(s)
    {}
};

template<typename T>
inline std::string toStr( T x )
{
    std::ostringstream o;
    if (!(o << x))
        throw BadConversion("stringify(double)");
    return o.str();
}