HEADERS       = \
    include/surface.h \
    include/glwidget.h \
    include/window.h

SOURCES       = \
    src/main.cpp \
    src/surface.cpp \
    src/glwidget.cpp \
    src/window.cpp

QT           += core gui widgets

RESOURCES    += shaders.qrc \
                textures.qrc

INCLUDEPATH  += include

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
