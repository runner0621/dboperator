#include "browser.h"
#include "test.h"
#include "public.h"

#include <QtCore>
#include <QtWidgets>
#include <QtSql>

static int start_window_ui(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int ret = DBOP_FAILURE;

#if defined (DBOP_TEST_ENABLE)
    ret = DbConnectTest();
#else
    ret = start_window_ui(argc, argv);
#endif

    return ret;
}


static int start_window_ui(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow mainWin;
    mainWin.setWindowTitle(QObject::tr("DbOperator"));

    Browser browser(&mainWin);
    mainWin.setCentralWidget(&browser);

    QMenu *optionMenu = mainWin.menuBar()->addMenu(QObject::tr("&Option"));
    optionMenu->addAction(QObject::tr("Add &Connection..."), &browser, SLOT(addConnection()));
    optionMenu->addSeparator();
    optionMenu->addAction(QObject::tr("&Quit"), &app, SLOT(quit()));

    QMenu *helpMenu = mainWin.menuBar()->addMenu(QObject::tr("&Help"));
    helpMenu->addAction(QObject::tr("About"), &browser, SLOT(about()));
    helpMenu->addAction(QObject::tr("About Qt"), qApp, SLOT(aboutQt()));

    QObject::connect(&browser, SIGNAL(statusMessage(QString)),
                     mainWin.statusBar(), SLOT(showMessage(QString)));

    mainWin.show();
    if (QSqlDatabase::connectionNames().isEmpty())
    {
        QMetaObject::invokeMethod(&browser, "addConnection", Qt::QueuedConnection);
    }

    return app.exec();
}
