#include "CReplayer.h"
#include "RecordData.h"
#include <fstream>
using namespace tlib::util;

CReplayer::CReplayer()
{
    // TODO: Read the name of the file from config
    m_datafile = "record_data.txt";
}

bool CReplayer::Begin()
{
    // Empty string stream
    m_data.str("");

    // Read file and write to stream
    ifstream f( m_datafile.c_str() );
    
    if( !f.is_open() ) return false;

      // get length of file:
    f.seekg (0, ios::end);
    int length = f.tellg();
    f.seekg (0, ios::beg);

    // allocate memory:
    char *buffer = new char [length];

    // read data as a block:
    f.read( buffer, length );
    f.close();

    m_data.str( buffer );
    m_data.clear();

    delete[] buffer;

    return true;
}

bool CReplayer::Read( RecordData &data )
{
    if( m_data.eof() || m_data.bad() || m_data.fail() ) return false;

    m_data >> data.key >> data.mouse >> data.x >> data.y;
    
    return true;
}