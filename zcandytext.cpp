#include <zcandytext.h>
#include <zobjectfunctions.h>
#include <zcandyprofile.h>

ZCandyText::~ZCandyText()
{
    if (Profile)
            delete Profile;
    if (TextBox)
            delete TextBox;
}
void ZCandyText::_generate(float pHeight, float pWidth, float pDepth, ZGLTextProfile *pTextProfile)
{
    ZCandy::_cloneFrom(generateCandy(pHeight,pWidth,pDepth,true,Name));

    TextBox=pTextProfile->newText();

    TextZone wTZ    = getTextZoneThin();
    TextBox->setBoxGLDimensions(wTZ.Width,wTZ.Height);
    TextBox->setBoxMarginsAll(5.0);
    TextBox->setBoxBorderColor(ZYellowBright);
    TextBox->setBoxBorderAlpha(1.0);
    TextBox->setBoxBorderWidth(1.5);
    TextBox->setBoxVisible(true);
    TextBox->setBoxFill(true);
    TextBox->setBoxBorder(true);
    TextBox->setBoxFillAlpha(1.0);
    TextBox->setTextFlag((uint16_t)RBP_Center|(uint16_t)RBP_WordWrap);

}

void ZCandyText::setPosition(glm::vec3 pPosition)
{
    ZCandy::setPosition(pPosition);
    TextZone wTZ    = getTextZone();
    glm::vec3 wTPos = pPosition + wTZ.ToTopLeft;
    TextBox->setPosition(wTPos);
}

