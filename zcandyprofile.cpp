#include <zcandyprofile.h>

#include <zcandytext.h>
#include <zobjectfunctions.h>
#include <zglresource.h>

_ZCandyProfileBase::_ZCandyProfileBase(const char* pName)
{
    Name=pName;
    GLResources->registerCandyProfile(this);
}

_ZCandyProfileBase::~_ZCandyProfileBase()
{

}

int _ZCandyProfileBase::store()
{
    std::string wXML;
    char wBuf[1000];

    wXML = "<zcandyprofile>\n";

    wXML = "<zcandyprofile>\n";

    sprintf (wBuf,
             "  <name>%s</name>\n",
             "  <width>%f</width>\n"
             "  <height>%f</height>\n"
             "  <depth>%f</depth>\n"
             ,
             Name,
             Width,
             Height,
             Depth);


    wXML += "</zcandyprofile>\n";
}
int _ZCandyProfileBase::load()
{

}



ZCandyText* ZCandyProfile::newCandy(const char* pName)
{
    if (Father==nullptr)
            {
            fprintf (stderr,
                     "ZCandyProfile::newCandy-E-PROFNULL cannot generate a text candy : profile has not be setup.\n");
            return nullptr;
            }
    ZCandyText* wCandy=new ZCandyText(this,pName);
    wCandy->_generate(Father->Height,Father->Width,Father->Depth,Father->TextProfile);


    for (int wi=0;wi < MaxShaderContext;wi++)
        if (Father->ShaderContext[wi])
            wCandy->ShaderContext[wi]= new ZShaderContext(*Father->ShaderContext[wi]);

    return wCandy;
}

ZCandyProfile* _ZCandyProfileBase::share()
{
    Shared++;
    return new ZCandyProfile(this);
}


