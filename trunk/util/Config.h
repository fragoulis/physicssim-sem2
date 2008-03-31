#pragma once
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>
using namespace std;

namespace tlib
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

    private:
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

} // end of namespace tlib