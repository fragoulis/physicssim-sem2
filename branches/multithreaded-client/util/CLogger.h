#pragma once
#include "tostring.h"
#include <iostream>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#   define _CLEAR_LOG   tlib::util::CLogger log;
#   define _LOG(str)    tlib::util::CLogger log(str);
#   define _SLOG(str)   tlib::util::CLogger log(str,true);
#else
#   define _CLEAR_LOG
#   define _LOG(str)
#   define _SLOG(str)
#endif

namespace tlib
{
namespace util
{

class CLogger
{
public:
    CLogger();
    CLogger( const string& entry, bool cout_only = false );

private:
    static const char *LOG_FILE;

}; // end CLogger

} // end namespace util
} // end namespace tlib