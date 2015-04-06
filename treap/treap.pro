TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11 -Wextra

SOURCES += main.cpp \
    ctreap.cpp \
    ctest.cpp \
    cstlpermutationstruct.cpp \
    cexception.cpp

HEADERS += \
    ctreap.h \
    ipermutationstructure.h \
    ctest.h \
    cstlpermutationstruct.h \
    cexception.h

