#include "CFileTimeSource.h"
#include <iostream>
using namespace tlib::time;

void CFileTimeSource::open( const char *sFile )
{
    if( !m_fsIn.is_open() )
        m_fsIn.open( sFile );
}

feed_t CFileTimeSource::get()
{
    if( !m_fsIn.is_open() || !m_fsIn.good() )
    {
        m_fsIn.close();
        return 0L;
    }

    // Read in values from the time file
    feed_t lClock;
    m_fsIn >> lClock;
    return lClock;
}