#ifndef ZCANDYPROFILE_H
#define ZCANDYPROFILE_H

#include <zgltextprofile.h>
#include <zshadercontext.h>



class ZCandyText;
class ZCandyProfile;
class _ZCandyProfileBase
{
public:
    _ZCandyProfileBase(const char* pName);
    _ZCandyProfileBase()
    {
        for (int wi=0;wi<MaxShaderContext;wi++)
                ShaderContext[wi]=nullptr;
    }
    ~_ZCandyProfileBase();

    void setDimensions(float pHeight,float pWidth,float pDepth) {Height=pHeight;Width=pWidth;Depth=pDepth;}

    ZCandyText* newCandy(const char* pName);

    /* stores xml definition for profile */
    int store();
    /* loads xml definition for profile */
    int load();

    ZCandyProfile* share();

    bool unShare() {Shared--;return (Shared>=0);}

    ZShaderContext* ShaderContext[MaxShaderContext];

    ZGLTextProfile*  TextProfile=nullptr;

    MatGen_Type     MatricesCreationFlag=MAT_Model | MAT_View | MAT_Projection | MAT_Normal;
    GDLC_type       GLSetupFlag=GLDC_Draw|GLDC_NormVisu;

    float Height=0.0f;
    float Width=0.0f;
    float Depth=0.0f;

    glm::vec3*      Color=nullptr;
    ZTexture*       Texture=nullptr;
    ZMaterial*      Material=nullptr;

    int Shared=0;
    const char* Name=nullptr; /* internal name */
};
/*

class ZCandyProfileInit
{
    ZCandyProfileInit() {}
    void setDimensions(float pHeight,float pWidth,float pDepth) {Father->Height=pHeight;Father->Width=pWidth;Father->Depth=pDepth;}


    ZCandyProfileBase* Father=nullptr;
};
*/

class ZCandyProfile
{
public:
    ZCandyProfile(_ZCandyProfileBase* pFather) {Father=pFather;}
    ~ZCandyProfile()
    {
        GLResources->deregisterCandyProfileBase(Father);
    }

    void createProfile(float pHeight,float pWidth,float pDepth) {Father->Height=pHeight;Father->Width=pWidth;Father->Depth=pDepth;}

    void setDimensions(float pHeight,float pWidth,float pDepth) {Father->Height=pHeight;Father->Width=pWidth;Father->Depth=pDepth;}

    int createShaderContextByName(const int pCtx,const char* pShaderName);
    int createShaderContextByRank(const int pCtx,const long pRank);

    int newTextShaderByName(const char* pShaderName);
    int newTextShaderByRank(const long pRank);

    int newTextBoxShaderByName(const int pCtx,const char* pShaderName);
    int newTextBoxShaderByRank(const int pCtx,const long pRank);

    void setTextWriter(ZGLTextProfile* pWriter)
    {
        if (Father->TextProfile)
                delete Father->TextProfile;
        Father->TextProfile=pWriter;
    }




    ZCandyText* newCandy(const char* pName);

    _ZCandyProfileBase* Father=nullptr;
};

#endif // ZCANDYPROFILE_H
