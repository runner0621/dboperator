#ifndef __QSQLCONNECTIONDIALOG_H__
#define __QSQLCONNECTIONDIALOG_H__

#include <QDialog>
#include <QMessageBox>

#include "ui_qsqlconnectiondialog.h"

class QSqlConnectionDialog: public QDialog
{
    Q_OBJECT
public:
    QSqlConnectionDialog(QWidget *parent = 0);
    ~QSqlConnectionDialog();

    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    bool useInMemoryDatabase() const;

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked() { reject(); }
    void on_dbCheckBox_clicked();

private:
    void UpdateDefaultSettings(void);
    Ui::QSqlConnectionDialogUi ui;
};

#endif //end of #ifndef __QSQLCONNECTIONDIALOG_H__
