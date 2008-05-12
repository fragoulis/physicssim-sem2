#pragma once
#include "Thread/CMutex.h"

struct input_t
{
    bool keys[256];
    bool mbutton;

	input_t() { memset( this, 0, sizeof(input_t) ); }
};

namespace tlib
{

class InputMap : public CMutex
{
public:
    InputMap();
    bool Get( input_t &data ) const;
    bool AccumInput(const input_t &data);
    bool Clear();

    bool SetMButton( bool mbutton );
    bool SetKey( int index, bool state );

private:
    void _Clear();
    input_t m_data;
};

} // end namespace tlib