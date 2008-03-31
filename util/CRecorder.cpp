#include "CRecorder.h"
#include "RecordData.h"
#include <fstream>
using namespace tlib::util;

CRecorder::CRecorder()
{
    // TODO: Read the name of the file from config
    m_timefile = "record_time.txt";
    m_datafile = "record_data.txt";
}

void CRecorder::End()
{
    // Write data to files
    ofstream f( m_timefile.c_str() );
    f << m_times.str();
    f.close();

    f.open( m_datafile.c_str() );
    f << m_data.str();
    f.close();

    // Empty string streams
    m_times.seekp(0);
    m_times.str("");

    m_data.seekp(0);
    m_data.str("");
}

void CRecorder::Record( RecordData &data )
{
    m_times << data.time << endl;
    m_data << data.key    << " "
           << data.mouse  << " "
           << data.x      << " "
           << data.y      << endl;
    
    Clear(data);
}

void CRecorder::Clear( RecordData &data ) const
{
    memset( &data, 0, sizeof(data) );
}