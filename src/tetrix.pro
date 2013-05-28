CONFIG += qt
QMAKE_CXXFLAGS += -std=c++11
HEADERS       = tetrixboard.h \
                 tetrixpiece.h \
				 tetrixwindow.h \
				 boardmodel.h \
    policy.h \
    state.h \
    boris.h \
    simpleaction.h \
    bossofboris.h \
    locoboss.h \
    greedyboss.h \
    gamemodel.h \
    complextetris.h \
    stochyboss.h \
    reinforcer.h \
    zuckermaas.h \
    vector.h
 SOURCES       = main.cpp \
                 tetrixboard.cpp \
                 tetrixpiece.cpp \
				 tetrixwindow.cpp \
				 boardmodel.cpp \
    policy.cpp \
    state.cpp \
    boris.cpp \
    simpleaction.cpp \
    bossofboris.cpp \
    locoboss.cpp \
    greedyboss.cpp \
    gamemodel.cpp \
    complextetris.cpp \
    stochyboss.cpp \
    reinforcer.cpp \
    zuckermaas.cpp \
    vector.cpp

 # install
 target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tetrix
 sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tetrix.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/tetrix
 INSTALLS += target sources


 symbian {
     TARGET.UID3 = 0xA000C606
     include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 }
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)
