#include "Config.h"
using namespace std;

namespace tlib
{
    Config::Config() {}
    Config::Config( const char *cfg_filename ) {
        open( cfg_filename );
    }

    // ------------------------------------------------------------------------
    bool Config::open( const char *cfg_filename )
    {
        m_File.open( cfg_filename );
        return m_File.is_open();
    }

    // ------------------------------------------------------------------------
    void Config::close() { m_File.close(); }

    // ------------------------------------------------------------------------
    int Config::loadBlock( const string &sTitle )
    {
        // Return if file is not open
        if( !m_File.is_open() ) return -1;

        string sCmpTitle;
        while( m_File >> sCmpTitle ) 
        {
            // Seach for a keyword with a dot(.) at the start
            // That is how a block title is represented
            if( (string(".")+sTitle) == sCmpTitle )
            {
                // Now save its data in std::map structure
                string sKey, sValue;
                while( m_File >> sKey >> sValue )
                {
                    // Quit when we reach another data block
                    if( sKey[0] == '.' || sKey[0] == ' ' ) break;
                    
                    m_Items[ sKey ] = sValue;
                }

                // Reset file before exiting
                _reset();

                // Return 1 for finding the block
                return 1;

            } // end if( )

        } // end while( )

        // Return 0 for finding nothing
        return 0;

    } // end _get()

    // ------------------------------------------------------------------------
    void Config::getInt( const string &sKey, 
                         int *out, 
                         const int nCount )
    {
        _get<int>( sKey, out, nCount );
    }

    // ------------------------------------------------------------------------
    void Config::getBool( const string &sKey, 
                          bool *out, 
                          const int nCount )
    {
        _get<bool>( sKey, out, nCount );
    }

    // ------------------------------------------------------------------------
    void Config::getChar( const string &sKey, 
                          char *out, 
                          const int nCount )
    {
        _get<char>( sKey, out, nCount );
    }

    // ------------------------------------------------------------------------
    void Config::getFloat( const string &sKey, 
                           float *out, 
                           const int nCount )
    {
        _get<float>( sKey, out, nCount );
    }

    // ------------------------------------------------------------------------
    void Config::getDouble( const string &sKey, 
                            double *out, 
                            const int nCount )
    {
        _get<double>( sKey, out, nCount );
    }

    // ------------------------------------------------------------------------
    void Config::getString( const string &sKey, string &sOut ) {
        sOut = m_Items[ sKey ];
    }

    

    // ------------------------------------------------------------------------
    void Config::_reset()
    {
        m_File.clear();
        m_File.seekg( 0, ios_base::beg );
    }

} // end of namespace tlib

