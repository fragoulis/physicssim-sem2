#pragma once
#include "CMutex.h"
#include <list>
using namespace tlib;

class SocketStream;
typedef std::list<SocketStream*> Streams;

/**
 * I need an easy way and not an ugly hack to have a common mutex between the 
 * scene's manager visual component list and the physics manager body list.
 */
class ClientMutex
{
public:
    static bool IsReadable() { return s_mutex.IsReadable(); }
    static bool IsWritable() { return s_mutex.IsWritable(); }

    static void ReleaseWrite() { s_mutex.ReleaseWrite(); }
    static void ReleaseAll() { s_mutex.ReleaseAll(); }

    static Streams clients;

private:
    static CMutex s_mutex;

};