#pragma once
#include <string>

namespace tlib
{
namespace gocs
{

/**
 * @class CHash
 * 
 * Stores an string in integer format
 */
class CHash
{
public:
    //! Returns the hash for the given string.
    //! The first time this operator is called, it caches the result and
    //! from then on returns the hashed value directly
    static unsigned _( const char *string )
    {
        unsigned hash = 0;
        unsigned len = (unsigned)strlen(string);
        unsigned i=0;
        for( ; i<len; ++i )
            hash += (i+1) * string[i];

        return hash;
    }

}; // end CHash

} // end namespace gocs
} // end namespace tlib