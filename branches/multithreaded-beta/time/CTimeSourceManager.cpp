#include "CTimeSourceManager.h"
using namespace tlib::time;

#include "CClockTimeSource.h"
#include "CFileTimeSource.h"

ITimeSource* CTimeSourceManager::Get( TimeSourceType iType )
{
    switch(iType)
    {
    case SRC_CLOCK: return new CClockTimeSource; break;
    case  SRC_FILE: return new CFileTimeSource; break;
    }

    assert(0); // should never come here

    return 0;
}