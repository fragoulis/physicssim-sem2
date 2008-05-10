#pragma once
#include "Thread/CMutex.h"

struct input_t
{
    bool keys[256];
    bool mbutton;
};

namespace tlib
{

class InputMap : public CMutex
{
public:
    InputMap();
    bool Get( input_t &data ) const;
    void AccumInput(const input_t &data);
    bool Clear();

    bool SetMButton( bool mbutton );
    bool SetKey( int index, bool state );

private:
    void _Clear();
    input_t m_data;
};

} // end namespace tlib