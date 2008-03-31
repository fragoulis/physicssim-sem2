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
 * @class CRecorder 
 *
 * Capturing and replaying class.
 * This class is responsible for reading and writing user input 
 * to/from a stream.
 */
class CRecorder : public Singleton<CRecorder>
{
private:
    //! The stream to write to
    ostringstream m_data, m_times;

    //! The associate files
    string m_datafile, m_timefile; 

public:
    //! Adds a record
    void Record( RecordData &data );

    //! Ends recording and writes data out to file
    void End();

    //! Empties the record data structure
    void Clear( RecordData &data ) const;

private:
    friend Singleton<CRecorder>;
    CRecorder();
};

typedef CRecorder InputRec;

} // end namespace util
} // end namespace tlib