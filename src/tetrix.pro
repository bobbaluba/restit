CONFIG += qt
QMAKE_CXXFLAGS += -std=c++11 -Wall -Werror -pedantic-errors -Wno-error=unused-local-typedefs
HEADERS = \
    boardmodel.h \
    state.h \
    boris.h \
    simpleaction.h \
    locoboss.h \
    gamemodel.h \
    complextetris.h \
    zuckermaas.h \
    vector.h \
    digitalclock.h \
    tetrisagent.h \
    tetrisboard.h \
    tetriswindow.h \
    greedyagent.h \
    stochyagent.h \
    tetronimo.h

SOURCES = main.cpp \
    boardmodel.cpp \
    state.cpp \
    boris.cpp \
    simpleaction.cpp \
    locoboss.cpp \
    gamemodel.cpp \
    complextetris.cpp \
    zuckermaas.cpp \
    vector.cpp \
    digitalclock.cpp \
    tetrisagent.cpp \
    greedyagent.cpp \
    tetriswindow.cpp \
    tetrisboard.cpp \
    stochyagent.cpp \
    tetronimo.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tetrix
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tetrix.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/tetrix
INSTALLS += target sources
