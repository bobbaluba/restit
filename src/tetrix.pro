CONFIG += qt
QMAKE_CXXFLAGS += -std=c++11 -Wall -Werror -pedantic-errors -Wno-error=unused-local-typedefs
HEADERS = tetrixboard.h \
    tetrixpiece.h \
    tetrixwindow.h \
    boardmodel.h \
    state.h \
    boris.h \
    simpleaction.h \
    bossofboris.h \
    locoboss.h \
    greedyboss.h \
    gamemodel.h \
    complextetris.h \
    stochyboss.h \
    zuckermaas.h \
    vector.h

SOURCES = main.cpp \
    tetrixboard.cpp \
    tetrixpiece.cpp \
    tetrixwindow.cpp \
    boardmodel.cpp \
    state.cpp \
    boris.cpp \
    simpleaction.cpp \
    bossofboris.cpp \
    locoboss.cpp \
    greedyboss.cpp \
    gamemodel.cpp \
    complextetris.cpp \
    stochyboss.cpp \
    zuckermaas.cpp \
    vector.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tetrix
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tetrix.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/tetrix
INSTALLS += target sources
