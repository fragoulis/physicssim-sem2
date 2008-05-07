#pragma once
#include "../Singleton.h"

namespace tlib
{
namespace time
{

    class ITimeSource;
    class Clock;

/**
 * @class CTimeSourceManager
 *
 * Basic time source manager.
 * The only thing it does is return a new time source depending on the time
 * source type. This class is created to decouple the main application and 
 * the clock from the time source and make switching between different time
 * sources and restarting the clock a trivial proccess.
 */
class CTimeSourceManager : public Singleton<CTimeSourceManager>
{
public:
    enum TimeSourceType { SRC_CLOCK, SRC_FILE };

private:
    friend Singleton<CTimeSourceManager>;
    friend Clock;
    ITimeSource* Get( TimeSourceType iType );
};

typedef CTimeSourceManager MGRTimeSrc;

} // end namespace time
} // end namespace tlib