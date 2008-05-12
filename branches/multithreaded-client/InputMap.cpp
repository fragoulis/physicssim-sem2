#include "InputMap.h"
using namespace tlib;

InputMap::InputMap()
{}

void InputMap::_Clear() 
{ 
    memset( &m_data, 0, sizeof(input_t) );
}

// ----------------------------------------------------------------------------
bool InputMap::Get( input_t &data ) const
{
    if( IsReadable() )
    {
        __TRY { data = m_data; }
        __FINALLY { ReleaseWrite(); }
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
void InputMap::AccumInput(const input_t &data)
{
    if( IsReadable() )
    {
        __TRY 
        { 
            for(int i=0; i<256; i++)
                m_data.keys[i] = m_data.keys[i] || data.keys[i];
        }
        __FINALLY { ReleaseWrite(); }
    }
}

// ----------------------------------------------------------------------------
bool InputMap::Clear()
{
    if( IsWritable() )
    {
        __TRY { _Clear(); }
        __FINALLY { ReleaseAll(); }
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool InputMap::SetMButton( bool mbutton )
{
    if( IsWritable() )
    {
        __TRY { m_data.mbutton = mbutton; }
        __FINALLY { ReleaseAll(); }
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool InputMap::SetKey( int index, bool state )
{
    if( IsWritable() )
    {
        __TRY { m_data.keys[index] = state; }
		__FINALLY { ReleaseAll(); }
        return true;
    }

    return false;
}