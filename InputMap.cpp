#include "InputMap.h"
using namespace tlib;

InputMap::InputMap()
{
    memset( &m_data, 0, sizeof(input_t) );
}

bool InputMap::Get( input_t &data ) const
{
    if( IsReadable() )
    {
        __try { 
           data = m_data;
        }
        __finally {
            ReleaseWrite();
        }

        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool InputMap::Set( const input_t &data )
{
    if( IsWritable() )
    {
        __try { 
           m_data = data;
        }
        __finally {
            ReleaseAll();
        }

        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool InputMap::SetMButton( bool mbutton )
{
    if( IsWritable() )
    {
        __try { 
           m_data.mbutton = mbutton;
        }
        __finally {
            ReleaseAll();
        }

        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool InputMap::SetKey( int index, bool state )
{
    if( IsWritable() )
    {
        __try { 
           m_data.keys[index] = state;
        }
        __finally {
            ReleaseAll();
        }

        return true;
    }

    return false;
}