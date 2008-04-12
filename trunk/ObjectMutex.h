#pragma once
#include "Thread/CMutex.h"

namespace tlib
{

/**
 * I need an easy way and not an ugly hack to have a common mutex between the 
 * scene's manager visual component list and the physics manager body list.
 */
class ObjectMutex
{
public:
    static bool IsReadable() { return s_mutex.IsReadable(); }
    static bool IsWritable() { return s_mutex.IsWritable(); }

    static void ReleaseWrite() { s_mutex.ReleaseWrite(); }
    static void ReleaseAll() { s_mutex.ReleaseAll(); }

private:
    static CMutex s_mutex;
};

} // end namespace tlib