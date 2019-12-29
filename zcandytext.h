#ifndef ZCANDYTEXT_H
#define ZCANDYTEXT_H

#include <zcandy.h>
#include <zglunicodetext.h>

class ZCandyText : public ZCandy
{
public:
    ZCandyText(const char* pName):ZCandy(pName) {}

    void generate() {}

};

#endif // ZCANDYTEXT_H
