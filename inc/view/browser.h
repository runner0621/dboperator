#ifndef __BROWSER_H__
#define __BROWSER_H__

#include <QWidget>
#include <QSqlTableModel>
#include "ui_browserwidget.h"
#include "db_manager.h"

class ConnectionWidget;
QT_FORWARD_DECLARE_CLASS(QTableView)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QSqlError)

class Browser: public QWidget, private Ui::Browser
{
    Q_OBJECT
public:
    Browser(QWidget *parent = 0);
    virtual ~Browser();
    void updateActions();

public slots:
    void exec();
    void addConnection();
    void currentChanged() { updateActions(); }
    void about();

    void on_submitButton_clicked()
    {
        exec();
        sqlEdit->setFocus();
    }
    void on_clearButton_clicked()
    {
        sqlEdit->clear();
        sqlEdit->setFocus();
    }

signals:
    void statusMessage(const QString &message);
private:
    const DbHandlerSt* mHandlePtr;
    //QString mDbPassword;
};

class CustomModel: public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CustomModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase()):QSqlTableModel(parent, db) {}
    QVariant data(const QModelIndex &idx, int role) const Q_DECL_OVERRIDE
    {
        if (role == Qt::BackgroundRole && isDirty(idx))
            return QBrush(QColor(Qt::yellow));
        return QSqlTableModel::data(idx, role);
    }
};

#endif //end of #ifndef __BROWSER_H__
