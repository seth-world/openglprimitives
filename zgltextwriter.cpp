#include <zgltextwriter.h>

#include <zshader.h>
#include <zglunicodetext.h>

ZGLTextWriter::ZGLTextWriter(GLenum pTextureEngine)
{
    TextureEngine=pTextureEngine;

    TextShader=new ZShader("zgltext.vs", "zgltext.fs", "UnicodeShader");
}


ZGLTextWriter::~ZGLTextWriter()
{
    while (TextList.count())
                delete TextList.popR();
    if (TextShader!=nullptr)
            delete TextShader;
}



GLUnicodeText* ZGLTextWriter::newText()
{
    GLUnicodeText* wT=new GLUnicodeText(this,TextureEngine);
    TextList.push(wT);
    return wT;
}

void ZGLTextWriter::_newBoxShader()
{
    BoxShader=new  ZShader("ztextbox.vs", "ztextbox.fs", "BoxShader");
}
