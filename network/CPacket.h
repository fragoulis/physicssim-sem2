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
    bool isSmall;
};

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
    char *m_buffer;
    int m_realSize;
    int m_index;
    int m_tries;

public:
    CPacket():
    m_tries(100)
    { 
        m_buffer = new char[DEFAULT_BUFFER_ALLOC];
        reset(); 
    }

    ~CPacket() { delete[] m_buffer; }

    // ------------------------------------------------------------------------
    //const char* buffer() const { return m_buffer; }
    int size() const { return m_index; }
    int realSize() const { return m_realSize; }

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
    void pull( Type *data, int size ) 
    {
        assert(m_index+size<m_realSize);
        memcpy( data, &m_buffer[m_index], size );
        m_index += size;
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

    void setSize( int size ) { m_realSize = size; }

    // ------------------------------------------------------------------------
    void reset() 
    {
        m_index = sizeof(int);	// the first four bytes are reserved to hold the structure size
        m_realSize = 0;
        memset( m_buffer, 0, DEFAULT_BUFFER_ALLOC );
    }

	// ------------------------------------------------------------------------
    int recv(SocketStream &ss)
    {
		readSize(ss);

		int bytesRead = 0;
		int i;
        for( i=0; (i<m_tries) && bytesRead<m_realSize; i++ ) {
		    bytesRead += ss.recv(&m_buffer[bytesRead+m_index], m_realSize-bytesRead);
	    }

        return bytesRead;
    }

	// ------------------------------------------------------------------------
	int send(SocketStream &ss, bool bfinalize = false)
	{
		if(bfinalize) finalize();

		int bytesSent = 0;
		for( int i=0; (i<m_tries) && bytesSent < size(); i++ ) {
			bytesSent += ss.send(&m_buffer[bytesSent], size()-bytesSent);
		}

		return bytesSent;
	}

	// ------------------------------------------------------------------------
	void finalize()
	{
		*(int*)&m_buffer[0] = m_index-sizeof(int);
	}

private:
	// ------------------------------------------------------------------------
    bool readSize(SocketStream &ss)
    {
		if( ss.recv(&m_buffer[0], m_index) < m_index ) return false;

		m_realSize = *(int*)&m_buffer[0];

		return true;
    }
};