#ifndef ZMATRICES_H
#define ZMATRICES_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for rotate, tranlate, etc.

#include <memory.h> // for memmove()

#include <zglresource.h>
#include <camera.h>

#define __MODEL_SHADER_NAME__ "mModel"
#define __VIEW_SHADER_NAME__ "mView"
#define __PROJECTION_SHADER_NAME__ "mProjection"
#define __NORMAL_SHADER_NAME__ "mNormal"

enum MatMgt :uint16_t {
    MM4_Nothing         = 0,
    MM4_ModelShared      = 1, /* Model matrix is local and must be computed locally : else reuse it as it is */
    MM4_ViewShared       = 2, /* View matrix is local and must be computed locally : else reuse it as it is */
    MM4_ProjShared       = 4,/* Project matrix is local and must be computed locally : else reuse it as it is */
    MM4_NormalShared       = 8,/* Normal matrix is local and must be computed locally : else reuse it as it is */
    MM4_AllShared       = MM4_ModelShared| MM4_ViewShared|MM4_ProjShared|MM4_NormalShared
};
enum MatSetup :uint16_t {
    MS4_Nothing         = 0,
    MS4_ModelSet      = 1, /* Model matrix is local and must be computed locally : else reuse it as it is */
    MS4_ViewSet       = 2, /* View matrix is local and must be computed locally : else reuse it as it is */
    MS4_ProjSet       = 4,/* Project matrix is local and must be computed locally : else reuse it as it is */
    MS4_NormalSet     = 8,/* Normal matrix is local and must be computed locally : else reuse it as it is */
    MS4_SetAll       = MS4_ModelSet| MS4_ViewSet|MS4_ProjSet|MS4_NormalSet
};

class ZMat4_M:public glm::mat4
{
friend class ZMat4;
private:
ZMat4_M()=default;

ZMat4_M(ZMat4_M &pIn) {memmove (this,&pIn,sizeof(pIn));}
ZMat4_M(ZMat4_M &&pIn) {memmove (this,&pIn,sizeof(pIn));}
ZMat4_M(glm::mat4 &&pIn) {memmove (this,&pIn,sizeof(pIn));}
ZMat4_M(glm::mat4 pIn) {memmove (this,&pIn,sizeof(pIn));}

ZMat4_M& operator= (const ZMat4_M& pIn){memmove (this,&pIn,sizeof(pIn));}
ZMat4_M& operator= (const ZMat4_M&& pIn){memmove (this,&pIn,sizeof(pIn));}
ZMat4_M& operator= (const glm::mat4& pIn){memmove (this,&pIn,sizeof(pIn));}
ZMat4_M& operator= (const glm::mat4&& pIn){memmove (this,&pIn,sizeof(pIn));}

ZMat4_M* share() {Shared++; return this;}
//~ZMat4_M() {delete _M;}
~ZMat4_M() {}

void unshare( ){Shared--;  }
private:

    int Shared=0;
};


class ZMat4{
    friend class ZMat4_M;
private:

    void clear()
    {
        if (Father)
                {
                Father->unshare();
                if (Father->Shared<0)
                        delete Father;
                }
        Father=nullptr;
    }

    ZMat4& _copyFrom(ZMat4& pIn)
    {
        clear();
        if (pIn.Father)
            {
            Father=pIn.Father->share();
            }
        return *this;
    }

    ZMat4(ZMat4& pM) {_copyFrom(pM);}
    ZMat4(ZMat4_M* pFather) {clear(); Father=pFather->share();}
public:
    ZMat4() {Father=new ZMat4_M;}
    ZMat4(glm::mat4 pM4) {Father=new ZMat4_M(pM4); }
    ~ZMat4()
    {
        Father->unshare();
        if (Father->Shared<0)
                delete Father;
    }
    ZMat4* share() {return new ZMat4(Father->share());}

    ZMat4& stdTranslate(glm::vec3 pPosition) {*Father=glm::translate(GLResources->Camera->getModel(), pPosition); return  *this;}

    ZMat4& rotate(float pAngle,glm::vec3 pAxis)
    {
        *Father=glm::rotate(*Father,pAngle,pAxis);
        return *this;
    }
    ZMat4& rotateDeg(float pAngle,glm::vec3 pAxis)
    {
        *Father=glm::rotate(*Father,glm::radians(pAngle),pAxis);
        return *this;
    }
    ZMat4& stdProjection()
    {
      *Father= glm::perspective(glm::radians(GLResources->Camera->Zoom),
                                GLResources->getWindowRatio(),
                                0.1f,
                                100.0f);
        return *this;
    }
    glm::mat4 get() {return (glm::mat4)*Father;}

    void setValue(const glm::mat4 pMat4) {  *Father=pMat4;}

    void setValue(const ZMat4 &pIn) { *Father=*pIn.Father;}

    void shareFrom(const ZMat4 &pIn) { clear(); Father=pIn.Father->share();}
    void shareFrom(const ZMat4* pIn) { clear(); if (pIn) Father=pIn->Father->share();}

    ZMat4& operator=(const glm::mat4 pGlmmat4) {*Father=pGlmmat4; return *this;}
    ZMat4& operator=(const ZMat4& pIn) {setValue(pIn); return *this;}

private:
    ZMat4_M* Father=nullptr;
};


class ZMatCtx
{

private:
    ZMatCtx& _cloneFrom(ZMatCtx& pIn)
    {
        shareMatrices(pIn.Model,pIn.Projection,pIn.View,pIn.Normal);

        if (Position)
                delete Position;
        Position=nullptr;
        if (pIn.Position)
                Position=new glm::vec3(pIn.getPosition());

        if (RotationAxis)
                delete RotationAxis;
        RotationAxis=nullptr;
        if (pIn.RotationAxis)
                RotationAxis=new glm::vec3(pIn.getRotationAxis());

        RotationAngle=pIn.RotationAngle;

        return * this;
    }

public:

    ZMatCtx()=default;

    ZMatCtx(ZMatCtx& pIn) {_cloneFrom(pIn);}


    ~ZMatCtx()
    {
    clear();
    }

    void clear();


    bool hasPosition()  {return Position!=nullptr ;}
    bool hasRotation()  {return RotationAxis!=nullptr && *RotationAxis!=glm::vec3(0.0f);}

    glm::vec3 getPosition()
    {
        if (!Position)
            return glm::vec3(0.0f);
        return *Position;
    }
    glm::vec3 getRotationAxis()
    {
        if (!RotationAxis)
            return glm::vec3(0.0f);
        return *RotationAxis;
    }

    void setPosition(glm::vec3 pPosition)
    {
        if (Position)
                delete Position;
        Position=new glm::vec3(pPosition);
    }
    void setRotationAxis(glm::vec3 pRotationAxis)
    {
        if (RotationAxis)
                delete RotationAxis;
        RotationAxis=new glm::vec3(pRotationAxis);
    }

    void setRotate(const float pAngle,const glm::vec3 pRotationAxis)
    {
        setRotationAxis(pRotationAxis);
        RotationAngle=pAngle;
    }





    /* Declares all matrices as inherited from meta object : this routine is called by meta object
        if input matrice pointer is set to nullptr, then matrice is left as it is without any change.
    */
    void shareMatrices (ZMat4* pModel,ZMat4* pView,ZMat4* pProjection, ZMat4* pNormal)
    {
        shareModel(pModel);
        shareView(pView);
        shareProjection(pProjection);
        shareNormal(pNormal);
        return;
    }

    void shareModel(ZMat4 *pModel)
    {
        if (pModel)
        {
            if (!Model)
                    Model=new ZMat4();
            Model->shareFrom(pModel);
            MatFlag|=MM4_ModelShared;
        }
/*        else
        {
            delete Model;
            Model=nullptr;
            MatFlag&=~MM4_ModelShared;
        }
*/
    }
    void shareView(ZMat4 *pView)
    {
        if (pView)
        {
            if (!View)
                    View=new ZMat4();
            View->shareFrom(pView);
            MatFlag|=MM4_ViewShared;
        }
 /*       else
        {
            delete View;
            View=nullptr;
            MatFlag&=~MM4_ViewShared;
        }*/
    }
    void shareProjection(ZMat4 *pProjection)
    {
        if (pProjection)
        {
            if (!Projection)
                    Projection=new ZMat4();
            Projection->shareFrom(pProjection);
            MatFlag|=MM4_ProjShared;
        }
/*        else
        {
            delete Projection;
            Projection=nullptr;
            MatFlag&=~MM4_ProjShared;
        }*/
    }
    void shareNormal(ZMat4 *pNormal)
    {
        if (pNormal)
        {
            if (!Normal)
                    Normal=new ZMat4();
            Normal->shareFrom(pNormal);
            MatFlag|=MM4_NormalShared;
        }
/*        else
        {
            delete Normal;
            Normal=nullptr;
            MatFlag&=~MM4_NormalShared;
        }*/
    }

    /**
     * @brief createMatrices creates matrices according flag value :
     *  set to true : Corresponding matrix is created, matrix is positionned to identity matrix.
     *  set to false : nothing is done (neither creation nor destruction).
     */
    void createMatrices (uint8_t pFlag);

    void createAllMatrices();
/*---Assign local values to matrices-------
    Once it is done, matrice is declared unshared (local vs global)
*/

    void setAllMatricesValue (ZMat4* pModel,ZMat4* pView,ZMat4* pProjection, ZMat4* pNormal);

    void setModelValue(glm::mat4 pModel);
    void setViewMatrice (glm::mat4  pView);
    void setProjectionMatrice (glm::mat4 pProjection);
    void setNormalMatrice (ZMat4* &&pNormal);


    /* matrices : set individual matrices as local to the object : */
    void createModel ();
    void createView ();
    void createProjection ();
    void createNormal ();



    ZMatCtx share()
            {
                Shared++;  // not done within copy constructor
                return ZMatCtx(*this);
            }
    void unShare() {Shared--;}

    void _preprocessGL(ZShader* wShader, uint8_t pFlag=MS4_SetAll);



    ZMat4 *     Model=nullptr;
    ZMat4*      Projection=nullptr;
    ZMat4*      View=nullptr;
    ZMat4*      Normal=nullptr;
    glm::vec3*  Position=nullptr;
    glm::vec3*  RotationAxis=nullptr;
    float       RotationAngle=0.0f;
    int         Shared=0;
    uint16_t  MatFlag=MM4_Nothing;
};// class ZMatCtx

#endif // ZMATRICES_H
