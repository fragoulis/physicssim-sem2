#pragma once
#include "../Singleton.h"
#include <sstream>
#include <string>
using namespace std;

struct RecordData;

namespace tlib
{
namespace util
{

/**
 * @class CReplayer 
 *
 * Capturing and replaying class.
 * This class is responsible for reading and writing user input 
 * to/from a stream.
 */
class CReplayer : public Singleton<CReplayer>
{
private:
    //! The stream to write to
    istringstream m_data;

    //! The associate files
    string m_datafile; 

public:
    //! Reads a record
    bool Read( RecordData &data );

    //! Ends recording and writes data out to file
    bool Begin();

private:
    friend Singleton<CReplayer>;
    CReplayer();
};

typedef CReplayer InputReplay;

} // end namespace util
} // end namespace tlib