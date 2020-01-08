#include <zgltextwriter.h>

#include <zshader.h>
#include <zglunicodetext.h>
#include <zshadercontext.h>
#include <zglconstants.h>


ZGLTextProfile::ZGLTextProfile(GLenum pTextureEngine)
{
    TextureEngine=pTextureEngine;

    TextShader=GLResources->getShaderByNamePtr(__TEXT_SHADER__);
    if (TextShader==nullptr)
            {
            fprintf (stderr,"ZGLTextWriter-F-SHADERNULL Required shader <%s> has not be loaded to GL resources. \n",__TEXT_SHADER__);
            exit (EXIT_FAILURE);
            }
//    TextShader=new ZShader("zgltext.vs", "zgltext.fs", "TextShader");
}


ZGLTextProfile::~ZGLTextProfile()
{
    while (TextList.count())
                delete TextList.popR();
    if (TextShader!=nullptr)
            delete TextShader;
    if (BoxShader[0])
            delete BoxShader[0];
    if (BoxShader[1])
            delete BoxShader[1];
}




ZShaderContext* ZGLTextProfile::getBoxShaderContext(const int pCtx)
{
    if (!BoxShader[pCtx])
                _newDefaultBoxShader(pCtx);
    return BoxShader[pCtx];
}

ZGLText* ZGLTextProfile::newText()
{
    ZGLText* wT=new ZGLText(this,TextureEngine);
    TextList.push(wT);
    return wT;
}

void ZGLTextProfile::_newDefaultBoxShader(const int pCtx)
{
    const char* wShName=__TEXTBOX_SHADER_FILL__;
    if (pCtx==Shape)
            wShName=__TEXTBOX_SHADER_SHAPE__;

    ZShader* wSh=GLResources->getShaderByNamePtr(wShName);
    if (wSh==nullptr)
            {
            fprintf (stderr,"ZGLTextWriter::_newBoxShaderShape-F-SHDNULL Required shader <%s> has not be registrated as GL resource.\n",
                     wShName);
            exit (EXIT_FAILURE);
            }

    BoxShader[pCtx]=new ZShaderContext(wSh);
}

int ZGLTextProfile::newBoxShader(const int pCtx, const char* pIntName)
{
    ZShader* wSh=GLResources->getShaderByNamePtr(pIntName);
    if (wSh==nullptr)
            {
            fprintf (stderr,"ZGLTextWriter::newBoxShaderFill-F-SHDNULL  Shader <%s> has not be registrated as GL resource.\n",
                     pIntName);
            return -1;
            }

    BoxShader[pCtx]=new ZShaderContext(wSh);
    return 0;
}

int ZGLTextProfile::newBoxShaderByRank(const int pCtx, const long pRank)
{
    ZShader* wSh=GLResources->getShaderByRankPtr(pRank);
    if (wSh==nullptr)
            {
            fprintf (stderr,"ZGLTextWriter::newBoxShaderFill-F-SHDNULL  Shader of rank <%ld> has not be registrated as GL resource.\n",
                     pRank);
            return -1;
            }

    BoxShader[pCtx]=new ZShaderContext(wSh);
    return 0;
}
