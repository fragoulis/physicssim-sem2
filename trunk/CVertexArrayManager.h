#pragma once
#include "Singleton.h"
#include <map>
#include <string>

namespace tlib
{

namespace util
{
    class IVertexArray;
}

class CVertexArrayManager : public Singleton<CVertexArrayManager>
{
private:
    typedef std::string VArrayId;
    typedef std::map< VArrayId, util::IVertexArray* > VAList;
    
    //! List of vertex arrays
    VAList m_Arrays;

    //! Currently active array
    util::IVertexArray* m_Active;

public:
    void Init();
    void Begin( const VArrayId &key );
    void Render();
    void End();

private:
    friend Singleton<CVertexArrayManager>;
    CVertexArrayManager();
    ~CVertexArrayManager();
};

typedef CVertexArrayManager MGRVertexArray;

} // end namespace tlib