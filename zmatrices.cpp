
#include <zmatrices.h>
#include <zshader.h>

void ZMatCtx::createMatrices (uint8_t pFlag)
{
    if (pFlag & MAT_Model)
            {
            if (Model)
                    delete Model;
            Model=new ZMat4(glm::mat4(1.0f));
            MatFlag &=~ MM4_ModelShared;
            }

    if (pFlag & MAT_View)
            {
            if (View)
                    delete View;
            View=new ZMat4(glm::mat4(1.0f));
            MatFlag &=~ MM4_ViewShared;
            }
    if (pFlag & MAT_Projection)
            {
        if (Projection)
                delete Projection;
            Projection=new ZMat4(glm::mat4(1.0f));
            MatFlag &=~ MM4_ProjShared;
            }
    if (pFlag & MAT_Normal)
            {
        if (Normal)
                delete Normal;
            Normal=new ZMat4(glm::mat4(1.0f));
            MatFlag &=~ MM4_NormalShared;
            }
    return;
}//createMatrices

void ZMatCtx::clear()
{
    if (Model)
            delete Model;
    Model=nullptr;
    if (Projection)
            delete Projection;
    Projection=nullptr;
    if (View)
            delete View;
    View=nullptr;
    if (Normal)
            delete Normal;
    Normal=nullptr;
    if (RotationAxis)
            delete RotationAxis;
    RotationAxis=nullptr;
    if (Position)
            delete Position;
    Position=nullptr;
}

void
ZMatCtx::_preprocessGL(ZShader* wShader,uint8_t pFlag)
{
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZMatCtx::_preprocessGL-E-NOSHD No valid shader while preprocessing GL environment.No action made.\n");
            return;
            }
/* setup matrices and transmit it to shader */

    /* if there is a request for local object translation : compute Model locally , if not, use what has been computed by meta object */
    if (MS4_ModelSet)
    if (Model)
        {
        if (Position ) /* object has a local position */
                {
            /* debug */
                float wX=Position->x;
                float wY=Position->y;
                float wZ=Position->z;
                wX=wY+wZ;
                /* end debug */
                Model->stdTranslate(*Position);
                }
            else
                Model->setValue(GLResources->Camera->getModel());

        if (RotationAxis)
            Model->rotate(RotationAngle,*RotationAxis);

        wShader->setMat4(__MODEL_SHADER_NAME__,Model->get());
        }//if (Model)


    /* if ZObject belongs to a ZMetaObject, then other matrices (view and projection) are supposed to be computed at
     * ZMetaObject level , then are given */
    if (MS4_ViewSet)
    if (View)
        {
        View->setValue(GLResources->Camera->GetViewMatrix());
        wShader->setMat4(__VIEW_SHADER_NAME__,View->get());
        }

    if (MS4_ProjSet)
    if (Projection)
        {
        Projection->setValue( glm::perspective(glm::radians(GLResources->Camera->Zoom),
                                                    GLResources->getWindowRatio(),
                                                    0.1f,
                                                    100.0f));
        wShader->setMat4(__PROJECTION_SHADER_NAME__,Projection->get());
        }
    if (MS4_NormalSet)
    if (Normal)
        {
        Normal->setValue( glm::transpose(glm::inverse(GLResources->Camera->GetViewMatrix()*GLResources->Camera->getModel())));
        wShader->setMat4(__NORMAL_SHADER_NAME__,Normal->get());
        }

}//_preprocessGL



void ZMatCtx::setAllMatricesValue (ZMat4* pModel,ZMat4* pView,ZMat4* pProjection, ZMat4* pNormal)
{
    if (Model)
            delete Model;
    Model=nullptr;
    if (pModel)
        Model->setValue(*pModel);

    if (View)
            delete View;
    View=nullptr;
    if (pView)
        View->setValue(*pView);

    if (Projection)
            delete Projection;
    Projection=nullptr;
    if (pProjection)
        Projection->setValue(*pProjection);

    if (Normal)
            delete Normal;
    Normal=nullptr;
    if (pNormal)
        Normal->setValue(*pNormal);

    MatFlag = MM4_AllShared;
    return;
}//setAllMatricesValue



/*---Assign local values to matrices-------
    Once it is done, matrice is declared unshared (local vs global)
*/

void ZMatCtx::setModelValue(glm::mat4 pModel)
{
    if (!Model)
            Model=new ZMat4(pModel);
    Model->setValue(pModel);
    MatFlag &= ~MM4_ModelShared;
}
void ZMatCtx::setViewMatrice (glm::mat4  pView)
{
    if (View)
            delete View;
    View->setValue(pView);
    MatFlag &= ~MM4_ViewShared;
}
void ZMatCtx::setProjectionMatrice (glm::mat4 pProjection)
{
    if (Projection)
            delete Projection;
    Projection->setValue(pProjection);
    MatFlag |= MM4_ProjShared;
}
void ZMatCtx::setNormalMatrice (ZMat4* &&pNormal)
{
    if (Normal)
            delete Normal;
    Normal=pNormal;
    MatFlag |= MM4_NormalShared;
}

void ZMatCtx::createAllMatrices()
{
    createModel ();
    createView();
    createProjection();
    createNormal();
}
/* matrices : set individual matrices as local to the object : */
void ZMatCtx::createModel ()
{
    if (Model!=nullptr)
                delete Model;
    Model=new ZMat4(glm::mat4(1.0f));
    MatFlag &=~MM4_ModelShared;
}
void ZMatCtx::createView ()
{
    if (View!=nullptr)
                delete View;
    View=new ZMat4;
    MatFlag &=~MM4_ViewShared;
}
void ZMatCtx::createProjection ()
{
    if (Projection!=nullptr)
                delete Projection;
    Projection=new ZMat4;
    MatFlag &=~MM4_ProjShared;
}
void ZMatCtx::createNormal ()
{
    if (Normal!=nullptr)
                delete Normal;
    Normal=new ZMat4;
    MatFlag &=~MM4_NormalShared;
}


