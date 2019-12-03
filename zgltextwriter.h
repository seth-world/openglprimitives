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

class GLUnicodeText;

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

    GLUnicodeText* newText();

    // Shader used for text rendering
    ZShader* TextShader=nullptr;
    /* shader used for text boxes rendering */
    ZShader* BoxShader=nullptr;

    void setModel(glm::mat4 pModel) {Model=pModel;}
    glm::mat4  getModel() {return Model;}

    void setView(glm::mat4 pView) {View=pView;}
    glm::mat4  getView() {return View;}

    void setProjection(glm::mat4 pProjection) {Projection=pProjection;}
    glm::mat4  getProjection() {return Projection;}

    int updateShader()
    {

    }

    ZShader* getBoxShader()
    {
        if (BoxShader==nullptr)
                    _newBoxShader();
        return BoxShader;
    }

    void _newBoxShader();

    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;

private:
    GLenum      TextureEngine=GL_TEXTURE0;

    zbs::ZArray<GLUnicodeText*> TextList;

}; // GLTextWriter

#endif // ZGLTEXTWRITER_H
