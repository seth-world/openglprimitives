#ifndef ZCANDYTEXT_H
#define ZCANDYTEXT_H

#include <zcandy.h>
#include <zglunicodetext.h>

class ZCandyProfile;

class ZCandyText : public ZCandy
{
public:
    ZCandyText(ZCandyProfile* pProfile,const char* pName):ZCandy(pName) {Profile=pProfile;}

    ZCandyText(ZCandy&pCandy) {_cloneFrom(pCandy);}

    ~ZCandyText();

    void _generate(float pHeight, float pWidth, float pDepth, ZGLTextProfile* pTextProfile);

    /* creates/recreates a new text instance for the Candy */
    int newText(const utf32_t* pText);

    /* centers Text Candy to the given position including text box */
    void setPosition(glm::vec3 pPosition);

    /* sets value to list of params for text box rendering */
    int setTextBox(const uint32_t pParam,...);

    /* sets value to list of params for text rendering */
    int setText(const uint32_t pParam,...);


    int setupGL();
    int draw();


    ZGLText* TextBox=nullptr;
    ZCandyProfile* Profile=nullptr;
};

#endif // ZCANDYTEXT_H
