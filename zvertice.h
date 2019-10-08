#ifndef ZVERTICES_H
#define ZVERTICES_H

#include <glm/glm.hpp>
#include <zglconstants.h>



//#pragma pack(push)
//#pragma pack(0)
class ZVertice
{
public:
    ZVertice() ;
/*    ZVertice(Vertice_type pPoint,Vertice_type pNormal,Color_type pColor,TextCoords_type pTextCoords);

    ZVertice(Vertice_type pPoint,Vertice_type pNormal,Color_type pColor);
*/
    ZVertice(Vertice_type pPoint);

    ZVertice(Vertice_type pPoint,Vertice_type pNormal);

    ZVertice(Vertice_type pVertice,Vertice_type pNormal,TextCoords_type pTextCoords);

    ZVertice(const ZVertice &pVertice)
    {
        point = pVertice.point;
        normal = pVertice.normal;
//        color = pVertice.color;
        textcoords=pVertice.textcoords;
    }

    void add(Vertice_type pVertice,Vertice_type pNormal,TextCoords_type pTextCoords);

    Vertice_type    point;
    Vertice_type    normal;
//    Color_type      color;
    TextCoords_type textcoords;
};

//#pragma pack(pop)

#endif // ZVERTICES_H
