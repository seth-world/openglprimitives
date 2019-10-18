#include <zvertice.h>

ZVertice::ZVertice()
{

}
/*
ZVertice::ZVertice(Vertice_type pPoint, glm::vec3 pNormal, Color_type pColor, TextCoords_type pTextCoords)
{
    point=pPoint;
    normal=pNormal;
//    color=pColor;
    textcoords=pTextCoords;
}
ZVertice::ZVertice(Vertice_type pPoint,Vertice_type pNormal,Color_type pColor)
{
    point=pPoint;
    normal=pNormal;
    color=pColor;
    textcoords=TextCoords_type(0.0f,0.0f);
}
ZVertice::ZVertice(Vertice_type pPoint,Vertice_type pNormal)
{
    point=pPoint;
    normal=pNormal;
    color=Color_type(0.0f,0.0f,0.0f);
    textcoords=TextCoords_type(0.0f,0.0f);
}*/
ZVertice::ZVertice(Vertice_type pPoint,Vertice_type pNormal,TexCoords_type pTextCoords)
{
    point=pPoint;
    normal=pNormal;
//    color=Color_type(0.0f,0.0f,0.0f);
    texcoords=pTextCoords;
}
ZVertice::ZVertice(Vertice_type pPoint)
{
    point=pPoint;
//    color=Color_type(0.0f,0.0f,0.0f);
    normal=Vertice_type(0.0f,0.0f,0.0f);
    texcoords=TexCoords_type(0.0f,0.0f);
}
