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
        zresource.cpp \
        zshader.cpp \
        zsphere.cpp \
        main.cpp \
        ../opengl/LearnOpenGL-master/src/stb_image.cpp \
        glad/src/glad.c \
        zobject.cpp \
        zobjectfunctions.cpp \
        ztexture.cpp \
        zvertice.cpp
INCLUDEPATH += glad/include \
                $$OPENGL_BASE/glad/include \
                $$OPENGL_BASE/glm \
                $$OPENGL_BASE/LearnOpenGL-master/includes \
                $$OPENGL_BASE/LearnOpenGL-master/build/configuration \
                $$TOOLSET_BASE \
                $$OPENGL_BASE/freeglut-3.2.1/include \
                $$DEVELOPMENT_BASE/ui3dg/ui3d-app

#static link with libzbaselib.a
LIBS += $$TOOLSET_BASE/zbaselib/bin/debug/libzbaselib.a

LIBS += $$OPENGL_BASE/freeglut-3.2.1/lib/libglut.so

LIBS +=  -lglfw -ldl -lpthread -lGL -lglut

HEADERS += \
    ../opengl/LearnOpenGL-master/includes/stb_image.h \
    Bmp.h \
    zglconstants.h \
    zresource.h \
    zsphere.h \
    glad/include/glad/glad.h \
    zshader.h \
    camera.h \
    zmaterial.h \
    zobject.h \
    zobjectfunctions.h \
    ztexture.h \
    zvertice.h

DISTFILES += \
    material.fs \
    zbasic_lighting.fs \
    zbasic_lighting.vs \
    zbasic_lighting_copy.fs \
    zbasic_lighting_copy.vs \
    zlamp.fs \
    zlamp.vs \
    zlighting.fs \
    zlighting.vs \
    zlighting_material.fs \
    zsphere.fs \
    zsphere.vs
