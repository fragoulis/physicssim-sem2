#include "CLogger.h"
using namespace tlib::util;

const char *CLogger::LOG_FILE = "logfile.txt";

CLogger::CLogger() 
{
    ofstream log( LOG_FILE );
}

CLogger::CLogger( const string& entry ) 
{
    ofstream log( LOG_FILE, ios_base::app );

    if( log.is_open() )
        log << entry << endl;

    cout << entry << endl;
}