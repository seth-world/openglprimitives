#include "zcandy.h"
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
void ZCandy::setupGLShape(ZShader* pShader)
{
    pShader->use();
    if(FrontShapeDesc!=nullptr)
            delete FrontShapeDesc;
    FrontShapeDesc = new ZGLObjDescriptor;

/* concatenate Front and back Shapes : Not in first approach */

    BackStart=(GLint) FrontShape.count(); /* first indice of BackShape after being concatenated  */

    FrontShape += BackShape;                    /* concatenate front and back to one unique set of vertices */
    FrontShapeDesc->Count = FrontShape.count(); /* total number of elements to draw */
/* Front shape setup */
/* GL buffers setup */
    glGenVertexArrays(1, &FrontShapeDesc->VAO);
    glGenBuffers(1, &FrontShapeDesc->VBO);

    glBindVertexArray(FrontShapeDesc->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, FrontShapeDesc->VBO);
    glBufferData(GL_ARRAY_BUFFER, FrontShape.usedSize(), FrontShape.data(), GL_STATIC_DRAW);

/* point position */
    glVertexAttribPointer(FrontShapeDesc->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)nullptr);
    glEnableVertexAttribArray(FrontShapeDesc->PositionAttribArray);
#ifdef __COMMENT__
/* Back shape setup */
/* GL buffers setup */

    glGenVertexArrays(1, &BackShapeDesc->VAO);
    glGenBuffers(1, &BackShapeDesc->VBO);

    glBindVertexArray(BackShapeDesc->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, BackShapeDesc->VBO);
    glBufferData(GL_ARRAY_BUFFER, BackShape.usedSize(), BackShape.data(), GL_STATIC_DRAW);

/* point position */
    glVertexAttribPointer(BackShapeDesc->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)0);
    glEnableVertexAttribArray(BackShapeDesc->PositionAttribArray);
#endif // __COMMENT__
// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // unbind
// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    pShader->release();
} //

void ZCandy::drawGLShape(ZShader* pShader)
{
    float wLineWidth;
    glGetFloatv(GL_LINE_WIDTH,&wLineWidth); /* get current line width */

    pShader->use();
    glLineWidth(2.0);
    glBindBuffer(GL_ARRAY_BUFFER, FrontShapeDesc->VBO);
    glBindVertexArray(FrontShapeDesc->VAO);
    /* draw front shape up until BackStart */
    glDrawArrays(GL_LINE_LOOP, 0 , BackStart);
    /* draw back shape from BackStart till end */
//    glBindVertexArray(FrontShapeDesc->VAO);
    glDrawArrays(GL_LINE_LOOP, (GLsizei)BackStart, (FrontShapeDesc->Count-BackStart));
    glLineWidth(wLineWidth);/* restore line width to its previous value */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    pShader->release();
}
