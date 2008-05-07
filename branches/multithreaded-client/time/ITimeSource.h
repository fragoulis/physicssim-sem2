#pragma once

namespace tlib
{
namespace time
{

// Typedef the source's return type here for future flexibility
// We call it 'feed' in the sence that the source feeds the clock
// with the new time...
typedef long feed_t;

/**
 * @class ITimeSource
 * 
 * This interface connects the central application clock with
 * a source to update its time from.
 */
class ITimeSource
{
public:
    virtual ~ITimeSource(){}
    virtual feed_t get() = 0;
};

} // end namespace time
} // end namespace tlib