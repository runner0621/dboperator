TEMPLATE        = app
TARGET          = dboperator

QT              += sql widgets

INCLUDEPATH     += inc \
                   inc/view \
                   3rd/oracle/ocilib/include \
                   src/db/plugin

HEADERS         = \
    inc/view/browser.h \
    inc/view/connectionwidget.h \
    inc/view/qsqlconnectiondialog.h \
    3rd/oracle/ocilib/include/ocilib.h \
    3rd/oracle/ocilib/include/ocilib.hpp \
    3rd/oracle/ocilib/include/ocilib_core.hpp \
    3rd/oracle/ocilib/include/ocilib_impl.hpp \
    inc/public.h \
    inc/test.h \
    inc/db_manager.h \
    inc/db_config.h \
    src/db/plugin/plg_oracle.h \
    inc/utilities.h

SOURCES         = \
    src/view/browser.cpp \
    src/view/connectionwidget.cpp \
    src/view/qsqlconnectiondialog.cpp \
    src/main.cpp \
    src/test/dbtest.c \
    src/db/db_manager.c \
    src/db/db_config.c  \
    src/db/plugin/plg_oracle.c \
    src/utilities/utilities.c

FORMS           = \
    ui/browserwidget.ui \
    ui/qsqlconnectiondialog.ui

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

#Add libs
#LIBS += $$PWD/oracle_sdk/lib/msvc/oci.lib
LIBS += $$PWD/3rd/oracle/ocilib/lib32/ociliba.lib \
        $$PWD/3rd/oracle/ocilib/lib32/ocilibw.lib


#Macro customization begin
#DEFINES += DBOP_TEST_ENABLE

#Macro customization end


# install
target.path = $$PWD/../out
INSTALLS += target
