#include "qsqlconnectiondialog.h"
#include "ui_qsqlconnectiondialog.h"
#include "db_manager.h"
#include "db_config.h"
#include "public.h"
#include <stdio.h>
#include <QSqlDatabase>


QSqlConnectionDialog::QSqlConnectionDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    QList<QString> qList = QList<QString>();

    int supportedDbCount = DbmGetSupportedDbCount();
    const char* dbDriverName = NULL;
    for (int i=0; i<supportedDbCount; ++i)
    {
        dbDriverName = DbmGetSupportedDbName(i);
        qList.append(QString(dbDriverName));
    }
    QStringList drivers = QStringList( qList);
    ui.comboDriver->addItems(drivers);

    //Init default settings
    int len = DBC_KEY_LEN_MAX;
    char value[DBC_KEY_LEN_MAX];

    int ret = DbcDefaultInstanceNameGet(value, len);
    DBOP_CHECK(DBOP_SUCCESS == ret);
    ui.editDatabase->setText(value);

    ret = DbcDefaultDbUserNameGet(value, len);
    DBOP_CHECK(DBOP_SUCCESS == ret);
    ui.editUsername->setText(value);

    ret = DbcDefaultDbIpGet(value, len);
    DBOP_CHECK(DBOP_SUCCESS == ret);
    ui.editHostname->setText(value);

    ret = DbcDefaultDbPortGet(value, len);
    DBOP_CHECK(DBOP_SUCCESS == ret);
    int port = 0;
    sscanf(value,"%d",&port);
    ui.portSpinBox->setValue(port);

    BOOL isCaseSensitive = TRUE;
    DbcDefaultSearchModeGet(&isCaseSensitive);
    ui.dbCheckBox->setChecked(isCaseSensitive ? true : false);
}

QSqlConnectionDialog::~QSqlConnectionDialog()
{
}

QString QSqlConnectionDialog::driverName() const
{
    return ui.comboDriver->currentText();
}

QString QSqlConnectionDialog::databaseName() const
{
    return ui.editDatabase->text();
}

QString QSqlConnectionDialog::userName() const
{
    return ui.editUsername->text();
}

QString QSqlConnectionDialog::password() const
{
    return ui.editPassword->text();
}

QString QSqlConnectionDialog::hostName() const
{
    return ui.editHostname->text();
}

int QSqlConnectionDialog::port() const
{
    return ui.portSpinBox->value();
}

void QSqlConnectionDialog::UpdateDefaultSettings(void)
{
    char value[DBC_KEY_LEN_MAX];

    QString dbDriver = ui.comboDriver->currentText();
    QString dbInstance = ui.editDatabase->text();
    QString dbUserName = ui.editUsername->text();
    QString dbPassword = ui.editPassword->text();
    QString dbIp = ui.editHostname->text();
    int dbPort = ui.portSpinBox->value();

    int ret = DbcDefaultDriverSet(dbDriver.toLatin1().data());
    ret = DbcDefaultInstanceNameSet(dbInstance.toLatin1().data());
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbcDefaultDbUserNameSet(dbUserName.toLatin1().data());
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbcDefaultDbIpSet(dbIp.toLatin1().data());
    DBOP_CHECK(DBOP_SUCCESS == ret);

    sprintf(value, "%d", dbPort);
    ret = DbcDefaultDbPortSet(value);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    DbcDefaultSearchModeSet(ui.dbCheckBox->isChecked() ? TRUE:FALSE);
}


void QSqlConnectionDialog::on_okButton_clicked()
{
    if (ui.comboDriver->currentText().isEmpty()) {
        QMessageBox::information(this, tr("No database driver selected"),
                                 tr("Please select a database driver"));
        ui.comboDriver->setFocus();
    } else {
        //Update db config
        UpdateDefaultSettings();

        //Open database connection
        //
        accept();
    }
}

void QSqlConnectionDialog::on_dbCheckBox_clicked()
{
    //ui.connGroupBox->setEnabled(!ui.dbCheckBox->isChecked());
}



