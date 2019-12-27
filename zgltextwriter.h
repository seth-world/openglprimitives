#ifndef ZGLTEXTWRITER_H
#define ZGLTEXTWRITER_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <zglconstants.h>

#include <ztexture.h>

#include <stdlib.h>  // for malloc, memset
#include <stdint.h>  // for uintxx_t



class ZShader;

class ZGLUnicodeText;

class ZTextBox;

class ZShaderContext;

/** For all dependent GLTextWriter generated, this object holds
 *   shader (generated)
 *   texture engine
 *   RFFU : matrices (view, model, projection)
 *
 *
 *  holds GLUnicodeText objects list and deletes it when GLFontWriter object is destroyed.
 *
 */
class ZGLTextWriter
{
public:


    // Constructor
    ZGLTextWriter(GLenum pTexture=GL_TEXTURE0);

    ~ZGLTextWriter();

    ZGLUnicodeText* newText();

    // Shader used for text rendering : text shader is used in standalone without shader context & rules
    ZShader* TextShader=nullptr;


    /* shader context used for text boxes rendering */
    ZShaderContext* BoxShader[2]={nullptr,nullptr};

    ZShaderContext* setBoxShader(const int pCtx) {return BoxShader[pCtx];}


    void setModel(glm::mat4 pModel) {Model=pModel;}
    glm::mat4  getModel() {return Model;}

    void setView(glm::mat4 pView) {View=pView;}
    glm::mat4  getView() {return View;}

    void setProjection(glm::mat4 pProjection) {Projection=pProjection;}
    glm::mat4  getProjection() {return Projection;}


    ZShader* getBoxShaderShape();
    ZShader* getBoxShader(const int pCtx);

    ZShaderContext* getBoxShaderContext(const int pCtx);

    void _newDefaultBoxShader(const int pCtx);
    int newBoxShader(const int pCtx, const char* pIntName);

    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;

private:
    GLenum      TextureEngine=GL_TEXTURE0;

    zbs::ZArray<ZGLUnicodeText*> TextList;

}; // GLTextWriter

#endif // ZGLTEXTWRITER_H
