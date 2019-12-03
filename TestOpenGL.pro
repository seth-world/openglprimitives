DEVELOPMENT_BASE = /home/gerard/Development
TOOLSET_BASE = /home/gerard/Development/zbasesystem
#ICU_BASE = /home/gerard/Development/usr
#ICU_LIB = /home/gerard/Development/usr/lib
#ICU_SRC = /home/gerard/Development/icu/source/common
#ICU_SRCi18n = /home/gerard/Development/icu/source/i18n

OPENGL_BASE = $$DEVELOPMENT_BASE/opengl


TEMPLATE = app
CONFIG += console c++17
#CONFIG -= app_bundle
CONFIG += qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += glad/include \
                $$OPENGL_BASE/glad/include \
                $$OPENGL_BASE/glm \
                $$OPENGL_BASE/LearnOpenGL-master/includes \
                $$OPENGL_BASE/LearnOpenGL-master/build/configuration \
                $$TOOLSET_BASE \
                $$OPENGL_BASE/freeglut-3.2.1/include \
                /usr/include/freetype2

#QMAKE_CXXFLAGS+= -std=c++17 -Wall -pthread

QMAKE_CXXFLAGS+= -std=c++17 -ansi -pedantic -pthread

#QMAKE_LFLAGS += -L/usr/lib/x86_64-linux-gnu/ -L$$TOOLSET_BASE/zbaselib/bin/debug/


#static link with libzbaselib.a
#LIBS += $$TOOLSET_BASE/zbaselib/bin/debug/libzbaselib.a

#dynamic link with libzbaselib.so
#LIBS += -L$$TOOLSET_BASE/zbaselib/bin/release/ -lzbaselib

#static link with libzbaselib.a

#LIBS += -Bstatic /usr/lib/x86_64-linux-gnu/libicuuc.a /usr/lib/x86_64-linux-gnu/libicudata.a
#LIBS += /usr/lib/x86_64-linux-gnu/libicui18n.a /usr/lib/x86_64-linux-gnu/libicuio.a /usr/lib/x86_64-linux-gnu/libicutu.a
#LIBS += /usr/lib/x86_64-linux-gnu/libicutest.a

#LIBS += -Bstatic $$ICU_LIB/libicuuc.a $$ICU_LIB/libicudata.a
#LIBS += $$ICU_LIB/libicui18n.a $$ICU_LIB/libicuio.a $$ICU_LIB/libicutu.a
#LIBS += $$ICU_LIB/libicutest.a


#LIBS+=  -licuuc -licudata -licui18n -licuio -licutu -licutest





#LIBS += $$OPENGL_BASE/freeglut-3.2.1/lib/libglut.so





#for icu

#for icu dynamic
#LIBS += -L/usr/lib/x86_64-linux-gnu/

#LIBS+= -L$$DEVELOPMENT_BASE/usr/lib -licuuc -licudata -licui18n -licuio -licutu -licutest
#LIBS+= -L/usr/lib/x86_64-linux-gnu -licuuc -licudata -licui18n -licuio -licutu -licutest

#LIBS+= -licuuc -licudata -licui18n -licuio -licutu -licutest

#LIBS+= $$DEVELOPMENT_BASE/usr/lib/libicuuc.so.62 \
#$$DEVELOPMENT_BASE/usr/lib/libicudata.so.62 \
#$$DEVELOPMENT_BASE/usr/lib/libicui18n.so.62 \
#$$DEVELOPMENT_BASE/usr/lib/libicuio.so.62 \
#$$DEVELOPMENT_BASE/usr/lib/libicutu.so.62 \
#$$DEVELOPMENT_BASE/usr/lib/libicutest.so.62

LIBS += -Wl,-Bstatic
LIBS += $$TOOLSET_BASE/zbaselib/bin/debug/libzbaselib.a

LIBS += -Wl,-Bdynamic
#LIBS+= -Wl,-Bdynamic -L/usr/lib/x86_64-linux-gnu -licuuc -licudata -licui18n -licuio -licutu -licutest
LIBS+=  -licuuc -licudata -licui18n -licuio -licutu -licutest

#LIBS += /usr/lib/x86_64-linux-gnu/libssl.a
#LIBS += /usr/lib/x86_64-linux-gnu/libcrypto.a


#LIBS += /usr/lib/x86_64-linux-gnu/libxml2.a

#LIBS += /home/gerard/Development/usr/lib/libicuuc.a
#LIBS += /home/gerard/Development/usr/lib/libicudata.a
#LIBS += /home/gerard/Development/usr/lib/libicui18n.a
#LIBS += /home/gerard/Development/usr/lib/libicuio.a
#LIBS += /home/gerard/Development/usr/lib/libicutu.a
#LIBS += /home/gerard/Development/usr/lib/libicutest.a



#LIBS +=  -lglfw -ldl -lpthread -lGL -lglut -lGLEW -lfreetype
LIBS +=   -ldl -lglfw -lpthread -lGL -lGLEW -lfreetype

LIBS+=  -lssl -lcrypto -lcrypt -lxml2

HEADERS += \
    ../opengl/LearnOpenGL-master/includes/stb_image.h \
    ../zbasesystem/ztoolset/zarray.h \
    Bmp.h \
    zboxcomponents.h \
    zcandy.h \
    zconfig.h \
    zfont.h \
    zglconstants.h \
    zglobjdescriptor.h \
    zglresource.h \
    zgltextwriter.h \
    zglunicode.h \
    zglunicodechar.h \
    zglunicodetext.h \
    zmetaobject.h \
    zsphere.h \
    zshader.h \
    camera.h \
    zmaterial.h \
    zobject.h \
    zobjectfunctions.h \
    ztexture.h \
    zvertice.h

SOURCES += \
        Bmp.cpp \
        zboxcomponents.cpp \
        zcandy.cpp \
        zfont.cpp \
        zglobjdescriptor.cpp \
        zglresource.cpp \
        zgltextwriter.cpp \
        zglunicodechar.cpp \
        zglunicodetext.cpp \
        zmetaobject.cpp \
        zshader.cpp \
        zsphere.cpp \
        main.cpp \
        ../opengl/LearnOpenGL-master/src/stb_image.cpp \
        zobject.cpp \
        zobjectfunctions.cpp \
        ztexture.cpp \
        zvertice.cpp

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
    shaders/ztextbox.fs \
    shaders/ztextbox.vs \
    shaders/ztextrenderer.fs \
    shaders/ztextrenderer.vs \
    shaders/ztextrenderer_copy.fs \
    shaders/ztexture.fs
