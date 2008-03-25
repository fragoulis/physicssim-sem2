#pragma once
#include "ITimeSource.h"
#include <fstream>
using std::ifstream;

namespace tlib
{
namespace time
{

class CFileTimeSource : public ITimeSource
{
private:
    // The input stream
    ifstream m_fsIn;

public:
    void open( const char *sFile );
    feed_t get();
};

} // end namespace time
} // end namespace tlib