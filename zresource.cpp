#include <zresource.h>

#include <ztexture.h>
#include <zobject.h>
#include <zshader.h>

ZGLResource _GLResources;
ZGLResource* GLResources=&_GLResources;

/* Freetype error description management as described per Freetype doc */
const char* getFTErrorString(const FT_Error error_code)
{
#undef FTERRORS_H_
#define FT_ERROR_START_LIST     switch ( error_code ) {
#define FT_ERRORDEF( e, v, s )    case v: return s;
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
    return "Unknown FT error";
}




/*
void ZResource::deregisterObject(ZObject *pObject)* pObject)
{
    for (long wi=0;wi<Objects.count();wi++)
        if (pObject==Objects[wi])
        {
            Objects[wi]->deleteGLContext();
            Objects.erase(wi);
            return;
        }
    return;
}

void ZResource::deregisterTexture(ZTexture* pObject)
{
    for (long wi=0;wi<Textures.count();wi++)
        if (pObject==Textures[wi])
        {
            Textures[wi]->deleteGLContext();
            Textures.erase(wi);
            return;
        }
    return;
}
*/
void ZGLResource::deregisterObject(ZObject* pObject)
{
    for (long wi=0;wi<Objects.count();wi++)
        if (pObject==Objects[wi])
        {
            Objects[wi]->deleteGLContext();
            Objects.erase(wi);
            return;
        }
    return;
}
void ZGLResource::deregisterShader(ZShader* pObject)
{
    for (long wi=0;wi<Shaders.count();wi++)
        if (pObject==Shaders[wi])
        {
            Shaders[wi]->deleteGLContext();
            Shaders.erase(wi);
            return;
        }
    return;
}
void ZGLResource::deregisterTexture(ZTexture* pObject)
{
    for (long wi=0;wi<Textures.count();wi++)
        if (pObject==Textures[wi])
        {
            Textures[wi]->deleteGLContext();
            Textures.erase(wi);
            return;
        }
    return;
}


void ZGLResource::cleanAll()
{
    while (Textures.count())
            Textures.popR()->deleteGLContext();
    while (Objects.count())
            Objects.popR()->deleteGLContext();;
    return;
}

ZShader ZGLResource::loadShader(const char*pVPath,const char*pFPath, const char*pGPath, const char*pName)
{
    return ZShader(pVPath,pFPath,pGPath,pName);
}


void ZGLResource::initFreeType()
{

    FT_Error wFTerr    = FT_Init_FreeType(&FreetypeLib);
    if (wFTerr!=FT_Err_Ok) // All functions return a value different than 0 whenever an error occurred
        {
            fprintf (stderr,"ZGLResource::Load-E Freetype error while initializing Freetype library.\n"
                     "    Error <%d>  <%s>\n",
                     wFTerr,
                     getFTErrorString( wFTerr));
            abort();
        }
}
void ZGLResource::closeFreeType()
{
    if (FreetypeLib != nullptr)
            FT_Done_FreeType(FreetypeLib);
    FreetypeLib=nullptr;
}
