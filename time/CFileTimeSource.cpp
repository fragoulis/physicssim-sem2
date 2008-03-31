#include "CFileTimeSource.h"
#include <fstream>
//#include <iostream>
using namespace tlib::time;

CFileTimeSource::CFileTimeSource()
{
    // Empty string stream
    m_times.str("");

    // Read file and write to stream
    ifstream f("record_time.txt");
    
    if( f.is_open() )
    {
        // get length of file:
        f.seekg (0, ios::end);
        int length = f.tellg();
        f.seekg (0, ios::beg);

        // allocate memory:
        char *buffer = new char [length];

        // read data as a block:
        f.read( buffer, length );
        f.close();

        m_times.str( buffer );
        m_times.clear();

        delete[] buffer;
    }
}

feed_t CFileTimeSource::get()
{
    feed_t feed;
    m_times >> feed;
    return feed;
}