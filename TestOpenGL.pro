DEVELOPMENT_BASE = /home/gerard/Development
TOOLSET_BASE = /home/gerard/Development/zbasesystem
ICU_BASE = /home/gerard/Development/usr
ICU_SRC = /home/gerard/Development/icu/source/common
ICU_SRCi18n = /home/gerard/Development/icu/source/i18n

OPENGL_BASE = $$DEVELOPMENT_BASE/opengl


TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Bmp.cpp \
        zboxcomponents.cpp \
        zcandy.cpp \
        zglobjdescriptor.cpp \
        zgltext.cpp \
        zglunicode.cpp \
        zmetaobject.cpp \
        zresource.cpp \
        zshader.cpp \
        zsphere.cpp \
        main.cpp \
        ../opengl/LearnOpenGL-master/src/stb_image.cpp \
        zobject.cpp \
        zobjectfunctions.cpp \
        ztextrenderer.cpp \
        ztexture.cpp \
        zvertice.cpp
INCLUDEPATH += glad/include \
                $$OPENGL_BASE/glad/include \
                $$OPENGL_BASE/glm \
                $$OPENGL_BASE/LearnOpenGL-master/includes \
                $$OPENGL_BASE/LearnOpenGL-master/build/configuration \
                $$TOOLSET_BASE \
                $$OPENGL_BASE/freeglut-3.2.1/include \
                /usr/include/freetype2

#static link with libzbaselib.a
LIBS += $$TOOLSET_BASE/zbaselib/bin/debug/libzbaselib.a

#LIBS += $$OPENGL_BASE/freeglut-3.2.1/lib/libglut.so

#LIBS +=  -lglfw -ldl -lpthread -lGL -lglut -lGLEW -lfreetype
LIBS +=  -lglfw -ldl -lpthread -lGL -lGLEW -lfreetype

HEADERS += \
    ../opengl/LearnOpenGL-master/includes/stb_image.h \
    ../zbasesystem/ztoolset/zarray.h \
    Bmp.h \
    zboxcomponents.h \
    zcandy.h \
    zconfig.h \
    zglconstants.h \
    zglobjdescriptor.h \
    zgltext.h \
    zglunicode.h \
    zmetaobject.h \
    zresource.h \
    zsphere.h \
    zshader.h \
    camera.h \
    zmaterial.h \
    zobject.h \
    zobjectfunctions.h \
    ztextrenderer.h \
    ztexture.h \
    zvertice.h

DISTFILES += \
    ./shaders/material.fs \
    ./shaders/zcolor.fs \
    ./shaders/zbasic_lighting.vs \
    ./shaders/zbasic_lighting_copy.fs \
    ./shaders/zbasic_lighting_copy.vs \
    ./shaders/zlamp.fs \
    ./shaders/zlamp.vs \
    ./shaders/zlighting.fs \
    ./shaders/zlighting.vs \
    ./shaders/zmaterial_lighting.fs \
    ./shaders/zsphere.fs \
    ./shaders/zsphere.vs \
    shaders/geometry.gs \
    shaders/normalvisu.fs \
    shaders/normalvisu.gs \
    shaders/normalvisu.vs \
    shaders/sphereshader.vs \
    shaders/textrenderer.fs \
    shaders/textrenderer.vs \
    shaders/zgltext.fs \
    shaders/zgltext.vs \
    shaders/zmaterial.fs \
    shaders/znormvisu.fs \
    shaders/znormvisu.vs \
    shaders/ztextrenderer.fs \
    shaders/ztextrenderer.vs \
    shaders/ztextrenderer_copy.fs \
    shaders/ztexture.fs
