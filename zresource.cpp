#include <zresource.h>

#include <ztexture.h>
#include <zobject.h>

ZGLResource _GLResources;
ZGLResource* GLResources=&_GLResources;

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
