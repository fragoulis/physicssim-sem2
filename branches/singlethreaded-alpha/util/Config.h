#pragma once
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>
using namespace std;

#define CFG_CLIENT_FILE         "client_cfg.txt"
#define CFG_SERVER_FILE         "server_cfg.txt"
#define CFG_VAR                 __cfg
#define CFG_OPEN(file)          tlib::util::Config CFG_VAR(file)
#define CFG_CLIENT_OPEN         CFG_OPEN(CFG_CLIENT_FILE)
#define CFG_SERVER_OPEN         CFG_OPEN(CFG_SERVER_FILE)
#define CFG_LOAD( block )       CFG_VAR.loadBlock( string(block) )

#define CFG_1i( key, value )  CFG_VAR.getInt      ( string(key), &value )
#define CFG_1b( key, value )  CFG_VAR.getBool     ( string(key), &value )
#define CFG_1f( key, value )  CFG_VAR.getFloat    ( string(key), &value )
#define CFG_1d( key, value )  CFG_VAR.getDouble   ( string(key), &value )
#define CFG_1c( key, value )  CFG_VAR.getChar     ( string(key), &value )
#define CFG_str( key, value ) CFG_VAR.getString   ( string(key), value )

// ------------------------------------------------------
#define CFG_GET_2( type, key, a, b ) { \
    type temp[2]; \
    CFG_VAR._get<type>( string(key), temp, 2 ); \
    a = temp[0]; \
    b = temp[1]; \
}
#define CFG_GET_2v( type, key, v ) { \
    CFG_VAR._get<type>( string(key), v, 2 ); \
}

#define CFG_GET_3( type, key, a, b, c ) { \
    type temp[3]; \
    CFG_VAR._get<type>( string(key), temp, 3 ); \
    a = temp[0]; \
    b = temp[1]; \
    c = temp[2]; \
}
#define CFG_GET_3v( type, key, v ) { \
    CFG_VAR._get<type>( string(key), v, 3 ); \
}

// ------------------------------------------------------
#define CFG_2f( key, a, b ) CFG_GET_2( float, key, a, b )
#define CFG_2fv( key, v ) CFG_GET_2v( float, key, v )

#define CFG_3f( key, a, b, c ) CFG_GET_3( float, key, a, b, c )
#define CFG_3fv( key, v ) CFG_GET_3v( float, key, v )

#define CFG_2i( key, a, b ) CFG_GET_2( int, key, a, b )
#define CFG_2iv( key, v ) CFG_GET_2v( int, key, v )


namespace tlib
{
namespace util
{

/**
 * This class is responsible for reading database values from 
 * a configuration file.
 * The file format supported is of [key=>value] type where
 * a key is a keyword which we search by and value is data of 
 * any type that we want to read.
 * The key and the value are separated by one or more spaces.
 * Each key-value pair is separated by a newline.
 */
class Config
{
private:
    // The file stream
    ifstream m_File;

    // The key items
    map < string, string > m_Items;

public:
    /**
     * Constructors
     */
    Config();
    Config( const char *cfg_filename );

    /**
     * Opens the stream and returns whether it was successfull
     */
    bool open( const char *cfg_filename );

    /**
     * Closes the stream
     */
    void close();

    /**
     * Loads a block of key-value paired data under the 
     * given block title
     */
    int loadBlock( const string &sTitle );

    /**
     * An simple method if we want a string value
     */
    void getString( const string &sKey, string &sOut );

    /**
     * Reads integers
     */
    void getInt( const string &sKey, 
                 int *out, 
                 const int nCount = 1 );

    /**
     * Reads booleans
     */
    void getBool( const string &sKey, 
                  bool *out, 
                  const int nCount = 1 );


    /**
     * Reads floats
     */
    void getFloat( const string &sKey, 
                   float *out, 
                   const int nCount = 1 );

    /**
     * Reads doubles
     */
    void getDouble( const string &sKey, 
                    double *out, 
                    const int nCount = 1 );

    /**
     * Reads chars
     */
    void getChar( const string &sKey, 
                  char *out, 
                  const int nCount = 1 );

//private:
    /**
     * Saves any number of values of any kind from a key-value
     * pair
     */
    template<typename T>
    void _get( const string &sKey, T *out, const int nCount );

    /**
     * Returns the file pointer to the beginning of the file
     */
    void _reset();

}; // end of class Config

// ------------------------------------------------------------------------
template<typename T>
void Config::_get( const string &sKey, T *out, const int nCount )
{
    // Create a string stream to easily read the values
    // from out data string
    istringstream instream;
    instream.clear();
    instream.str( m_Items[sKey] );

    // dummy character to read delimiter
    char d;
    for( int i=0; i<nCount; ++i ) {
        instream >> out[i] >> d;
    }
    
} // end of _get

} // end of namespace util
} // end of namespace tlib