#ifndef ZVERTICES_H
#define ZVERTICES_H

#include <glm/glm.hpp>
#include <zglconstants.h>



//#pragma pack(push)
//#pragma pack(0)
class ZVertice
{
private:
    void _cloneFrom(const ZVertice& pIn)
    {
        point=pIn.point;
        normal=pIn.normal;
        texcoords=pIn.texcoords;
    }
public:
    ZVertice() ;
/*    ZVertice(Vertice_type pPoint,Vertice_type pNormal,Color_type pColor,TextCoords_type pTextCoords);

    ZVertice(Vertice_type pPoint,Vertice_type pNormal,Color_type pColor);
*/
    ZVertice(Vertice_type pPoint);

    ZVertice(Vertice_type pPoint,Vertice_type pNormal);

    ZVertice(Vertice_type pVertice,Vertice_type pNormal,TexCoords_type pTextCoords);

    ZVertice (float px,float py,float pz,float pnx,float pny,float pnz,float ps, float pt )
    {
      point=Vertice_type(px,py,pz);
      normal=Vertice_type(pnx,pny,pnz);
      texcoords=TexCoords_type(ps,pt);
    }

    ZVertice(const ZVertice &pVertice)
    {
        point = pVertice.point;
        normal = pVertice.normal;
//        color = pVertice.color;
        texcoords=pVertice.texcoords;
    }

    ZVertice& operator = (const ZVertice& pIn) {_cloneFrom(pIn); return *this;}


    Vertice_type    point;
    Vertice_type    normal;
//    Color_type      color;
    TexCoords_type texcoords;
};

//#pragma pack(pop)

#endif // ZVERTICES_H
