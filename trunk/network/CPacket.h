#pragma once
#include "../Network/SocketStream.h"
#include "../GOCS/CTransform.h"
#include <vector>
#include <string>
#include <cassert>

typedef tlib::CTransform         transform_t;
typedef std::vector<transform_t> transforms_t;
typedef tlib::math::Vec3f        position_t;
typedef std::vector<position_t>  positions_t;
typedef std::string              stream_t;

struct camera_t
{
    position_t position;
    position_t lookat;
};

struct sphere_t
{
    position_t position;
    int type;
};
typedef std::vector<sphere_t> spheres_t;

#define TO_CHAR( object ) (char*)&object

const int DEFAULT_BUFFER_ALLOC = 1000000;

/**
 * The data packet we send over the network.
 * This packet will contain all the data the client needs to know.
 * It is constructed by two parts: the fixed and the dynamic. 
 * The fixed part contains data regarding walls, cloth, shelf, camera and input.
 * The dynamic part contains the spheres' data.
 *
 * vector       = 3 x 4 = 12 bytes
 * quaternion   = 4 * 4 = 16 bytes
 * transform    = vector + quaternion = 28 bytes
 * 
 *** Packet Structure *********************************************************
 * 168 bytes = transform for six walls (cube position and rotation)
 *   1 byte  = (boolean) which camera to use
 *   1 byte  = (boolean) whether to use the shelf or the cloth
 *  28 bytes = shelf transform
 *   N bytes = cloth vertex positions, 12 bytes x number of vectices
 *   N bytes = input
 *   M bytes = ball positions and type, 16 bytes x number of spheres
 *
 */
class CPacket
{
private:
    char m_buffer[DEFAULT_BUFFER_ALLOC];
    int m_realSize;
    int m_index;
    int m_tries;

public:
    CPacket():
    m_tries(3)
    { 
        reset(); 
    }

    // ------------------------------------------------------------------------
    template<class Type>
    void pull( Type &data ) 
    {
        //assert(m_index+sizeof(Type)<m_realSize);
        data = *(Type*)&m_buffer[m_index];
        m_index += sizeof(Type);
    }

    // ------------------------------------------------------------------------
    template<class Type>
    void push( const Type &data ) 
    {
        assert(m_index+sizeof(Type)<DEFAULT_BUFFER_ALLOC);
        *(Type*)&m_buffer[m_index] = data;
        m_index += sizeof(Type);
    }

    // ------------------------------------------------------------------------
    template<class Type>
    void push( const Type *data, int size ) 
    {
        assert(m_index+size<DEFAULT_BUFFER_ALLOC);
        memcpy( &m_buffer[m_index], data, size );
        m_index += size;
    }

    // ------------------------------------------------------------------------
    const char* buffer() const { return m_buffer; }
    int size() const { return m_index; }
    int realSize() const { return m_realSize; }

    void setSize( int size ) { m_realSize = size; }

    // ------------------------------------------------------------------------
    void reset() 
    {
        m_index = 0;
        m_realSize = 0;
        memset( m_buffer, 0, DEFAULT_BUFFER_ALLOC );
    }

    void recv(SocketStream &ss)
    {
        for( int i=0; (i<m_tries) && (m_realSize<DEFAULT_BUFFER_ALLOC); i++ ) {
		    m_realSize += ss.recv(&m_buffer[m_realSize], DEFAULT_BUFFER_ALLOC);
	    }
    }
};