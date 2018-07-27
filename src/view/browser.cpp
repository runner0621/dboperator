#include "browser.h"
#include "qsqlconnectiondialog.h"
#include "public.h"
#include "db_manager.h"
#include "db_config.h"

#include <QtWidgets>
#include <QtSql>

#define    DBOP_RESULT_OUTPUT_FILE_PATH      "SearchResult.txt"

Browser::Browser(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    //Init & open database
    mHandlePtr = DbmInit(DBOP_NAME_ORACLE);
    DBOP_CHECK(NULL != mHandlePtr);
    submitButton->setEnabled(FALSE);

    emit statusMessage(tr("Ready."));
}

Browser::~Browser()
{
    int ret = mHandlePtr->Close();
    DBOP_CHECK(DBOP_SUCCESS == ret);
}

void Browser::exec()
{
    QString  str = sqlEdit->toPlainText();
    QByteArray ba = str.toLatin1();
    char* queryData = ba.data();

    DBOP_LOG(DBOP_LOG_INFO, "sqlEdit data:%s", queryData);

    //sqlEdit->Tex
    //QTextEdit *dd;

    //Get DB search mode
    BOOL isCaseSensitive = TRUE;
    DbcDefaultSearchModeGet(&isCaseSensitive);

    int ret = DbmDatabaseSearch(mHandlePtr,
            DBOP_CDT_TEXT,
            queryData,
            (TRUE==isCaseSensitive) ? DBOP_SEARCH_MODE_CASE_SENSITIVE : DBOP_SEARCH_MODE_IGNORE_CASE,
            DBOP_RESULT_OUTPUT_FILE_PATH);

    DBOP_CHECK(DBOP_SUCCESS == ret);

    QMessageBox::information(this, tr("Result"),
                             (DBOP_SUCCESS == ret) ? tr("Query Succesfully!") : tr("Query Failed!"));

    updateActions();
}

void Browser::addConnection()
{
    QSqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    int ret = DBOP_FAILURE;
    do {
        QString password = dialog.password();

        //Init default settings
        int len = DBC_KEY_LEN_MAX;
        char dbService[DBC_KEY_LEN_MAX*2];
        char dbInstance[DBC_KEY_LEN_MAX];
        char dbUserName[DBC_KEY_LEN_MAX];
        char dbIp[DBC_KEY_LEN_MAX];
        char dbPort[DBC_KEY_LEN_MAX];

        ret = DbcDefaultInstanceNameGet(dbInstance, len);
        DBOP_CHECK_THEN_BREAK(DBOP_SUCCESS == ret);

        ret = DbcDefaultDbUserNameGet(dbUserName, len);
        DBOP_CHECK_THEN_BREAK(DBOP_SUCCESS == ret);

        ret = DbcDefaultDbIpGet(dbIp, len);
        DBOP_CHECK_THEN_BREAK(DBOP_SUCCESS == ret);

        ret = DbcDefaultDbPortGet(dbPort, len);
        DBOP_CHECK_THEN_BREAK(DBOP_SUCCESS == ret);

        snprintf(dbService, DBC_KEY_LEN_MAX*2, "%s:%s/%s", dbIp, dbPort, dbInstance);

        ret = mHandlePtr->Open(dbService, dbUserName, password.toLatin1().data());
        DBOP_CHECK_THEN_BREAK(DBOP_SUCCESS == ret);

        ret = DBOP_SUCCESS;
    } while (0); //End of do {} while (0)

    if (DBOP_SUCCESS != ret)
    {
        submitButton->setEnabled(FALSE);
        QMessageBox::warning(this, tr("Unable to open database"),
                             tr("An error occurred while opening the connection: "));
    } else {
        submitButton->setEnabled(TRUE);
    }
}

void Browser::updateActions()
{
    /*
    QSqlTableModel * tm = qobject_cast<QSqlTableModel *>(table->model());
    bool enableIns = tm;
    bool enableDel = enableIns && table->currentIndex().isValid();

    insertRowAction->setEnabled(enableIns);
    deleteRowAction->setEnabled(enableDel);

    fieldStrategyAction->setEnabled(tm);
    rowStrategyAction->setEnabled(tm);
    manualStrategyAction->setEnabled(tm);
    submitAction->setEnabled(tm);
    revertAction->setEnabled(tm);
    selectAction->setEnabled(tm);

    if (tm) {
        QSqlTableModel::EditStrategy es = tm->editStrategy();
        fieldStrategyAction->setChecked(es == QSqlTableModel::OnFieldChange);
        rowStrategyAction->setChecked(es == QSqlTableModel::OnRowChange);
        manualStrategyAction->setChecked(es == QSqlTableModel::OnManualSubmit);
    }
    */
}

void Browser::about()
{
    QMessageBox::about(this, tr("About"), tr("Tool for analyzing database, created by leo.yu!"));
}


