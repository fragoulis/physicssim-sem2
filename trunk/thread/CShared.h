#pragma once

namespace tlib
{

template< class Type >
class CShared
{
private:
    Type m_data;
    bool m_bReadable, m_bWritable;

public:
    CShared() { Enable(); }
    CShared( const Type &data ) { SetData(data); }

    void GetData( Type &data ) const 
    {
        data = m_data;
        EnableWriting();
    }
        
    void SetData( const Type &data )
    {
        m_data = data;
        Disable();
    }

    bool ReadLock()
    { 
        if( m_bReadable ) 
        {
            DisableWriting();
            return true;
        }

        return false;
    }

    bool WriteLock()
    { 
        if( m_bWritable ) 
        {
            Disable();
            return true;
        }

        return false;
    }

    void operator = ( const Type &data ) { SetData(data); }

private:
    void Enable()   { m_bReadable = m_bWritable = true; }
    void Disable()  { m_bReadable = m_bWritable = false; }

    void EnableWriting()    { m_bWritable = true; }
    void DisableWriting()   { m_bWritable = false; }

    void EnableReading()    { m_bReadable = true; }
    void DisableReading()   { m_bReadable = false; }
};

typedef CShared<bool>       bool_s;
typedef CShared<int>        int_s;
typedef CShared<float>      float_s;
typedef CShared<double>     double_s;

} // end namespace tlib