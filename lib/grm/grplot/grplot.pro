GRDIR = $$(GRDIR)
isEmpty(GRDIR) {
  GRDIR = /usr/local/gr
}
QT += widgets core
CONFIG += c++17
DEFINES += GRDIR=\\\"$(GRDIR)\\\"
# Qt versions < 5.12 ignore `CONFIG`, so repeat the language flag here.
# Only using `QMAKE_CXXFLAGS` does not work since newer Qt installations set
# conflicting language flags themselves without setting `CONFIG`.
QMAKE_CXXFLAGS += -std=c++17 $$(EXTRA_CXXFLAGS)
QMAKE_LFLAGS += $$(EXTRA_LDFLAGS)
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
HEADERS += grplot_widget.hxx grplot_mainwindow.hxx util.hxx qtterm/grm_args_t_wrapper.h qtterm/receiver_thread.h
SOURCES += grplot_widget.cxx grplot.cxx grplot_mainwindow.cxx util.cxx qtterm/grm_args_t_wrapper.cpp qtterm/receiver_thread.cpp
INCLUDEPATH += ../include ../../gr
if (macx) {
    if (exists(../libGRM.dylib)) {
      LIBS += -L.. -L../../gr -lGRM -lGR
    } else {
      LIBS += -L$(GRDIR)/lib -lGRM -lGR
    }
    # On macOS, the grplot executable is located in `$(GRDIR)/Applications/grplot.app/Contents/MacOS`
    # and we need to resolve `libGRM.dylib` in `$(GRDIR)/lib`
    QMAKE_RPATHDIR += ../../../../lib
} else {
    if (exists(../libGRM.so)) {
      LIBS += -L.. -L../../gr -lGRM -lGR -Wl,-rpath-link,../../gr -Wl,-rpath-link,../../gr3
    } else {
      LIBS += -L$(GRDIR)/lib -lGRM -lGR -Wl,-rpath-link,$(GRDIR)/lib
    }
    # On every other system, the grplot executable is located in `$(GRDIR)/bin`
    # and we need to resolve `libGRM.so` in `$(GRDIR)/lib`
    QMAKE_RPATHDIR += ../lib
}
