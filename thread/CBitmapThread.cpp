#include "CBitmapThread.h"
#include "../CSceneManager.h"
#include "../Util/Config.h"
#include "../Util/tostring.h"

// -----------------------------------------------------------------------------
void CBitmapThread::Run( void *lpArgs )
{
    StringList::iterator image_iterator = m_images.begin();
    while(IsRunning())
    {
        MGRScene::Get().LoadNewBackplaneTexture( (*image_iterator).c_str() );

        // Advance iterator to use next image
        if( m_time > m_changeInterval )
        {
            m_time = 0;
            ++image_iterator;
            if( image_iterator == m_images.end() )
                image_iterator = m_images.begin();
        }

        m_time++;
        Sleep(m_sleep);
    }

} // Run()

// ----------------------------------------------------------------------------
void CBitmapThread::OnStart()
{
    // Read the path of the images and the images' filenames
    CFG_CLIENT_OPEN;
    CFG_LOAD("Bitmap_Reader");
    CFG_1i("ChangeInterval", m_changeInterval);
    CFG_1i("Sleep", m_sleep);

    std::string path;
    CFG_str("Path", path);

    int number;
    CFG_1i("Number", number);

    m_images.resize(number);

    for( int i=0; i<number; i++ )
    {
        std::string key = "Image"+toStr<int>(i+1), file;
        CFG_str( key, file );

        m_images[i] = path + file;
    }

    m_time = 0;
}