#pragma once
#include <ctime>
#include "ITimeSource.h"

namespace tlib
{
namespace time
{

class CClockTimeSource : public ITimeSource
{
public:
    feed_t get() { return clock(); }
};

} // end namespace time
} // end namespace tlib