#pragma once
#include "Thread/ISharedData.h"

struct input_t
{
    bool keys[256];
    bool mbutton;
};

namespace tlib
{

class InputMap : public ISharedData
{
public:
    InputMap();
    bool Get( input_t &data ) const;
    bool Set( const input_t &data );

    bool SetMButton( bool mbutton );
    bool SetKey( int index, bool state );

private:
    input_t m_data;
};

} // end namespace tlib