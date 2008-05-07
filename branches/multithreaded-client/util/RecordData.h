#pragma once

struct RecordData
{
    long time;  // the current clock time
    int key;    // the keyboard key pressed, if any
    int mouse;  // whether the mouse button is down
    int x, y;   // mouse coordinates
};