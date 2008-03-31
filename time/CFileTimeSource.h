#pragma once
#include "ITimeSource.h"
#include <sstream>
using namespace std;

namespace tlib
{
namespace time
{

class CFileTimeSource : public ITimeSource
{
private:
    // The input stream
    istringstream m_times;

public:
    CFileTimeSource();
    feed_t get();
};

} // end namespace time
} // end namespace tlib