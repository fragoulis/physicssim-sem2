#include "InputMap.h"
using namespace tlib;

InputMap::InputMap()
{
    _Clear();
}

void InputMap::_Clear() 
{ 
    memset( &m_data, 0, sizeof(input_t) );
}

bool InputMap::Get( input_t &data ) const
{
    if( IsReadable() )
    {
        __TRY 
        { 
           data = m_data;
        }
        __FINALLY 
        {
            ReleaseWrite();
        }

        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool InputMap::Clear()
{
    if( IsWritable() )
    {
        __TRY 
        { 
            _Clear();
        }
        __FINALLY 
        {
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