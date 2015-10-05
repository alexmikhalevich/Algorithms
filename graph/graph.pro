TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    test.cpp

HEADERS += \
    cpathfinder.hpp \
    bus.hpp \
    corientedgraph.hpp \
    gas_stations.hpp

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lgtest

INCLUDEPATH += $$PWD/../../../../../usr/include/gtest
DEPENDPATH += $$PWD/../../../../../usr/include/gtest

