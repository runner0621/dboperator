#ifndef __CONNECTIONWIDGET_H__
#define __CONNECTIONWIDGET_H__

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(QSqlDatabase)
QT_FORWARD_DECLARE_CLASS(QMenu)

class ConnectionWidget: public QWidget
{
    Q_OBJECT
public:
    ConnectionWidget(QWidget *parent = 0);
    virtual ~ConnectionWidget();

    QSqlDatabase currentDatabase() const;

signals:
    void tableActivated(const QString &table);
    void metaDataRequested(const QString &tableName);

public slots:
    void refresh();
    void showMetaData();
    void on_tree_itemActivated(QTreeWidgetItem *item, int column);
    void on_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    void setActive(QTreeWidgetItem *);

    QTreeWidget *tree;
    QAction *metaDataAction;
    QString activeDb;
};

#endif //end of #ifndef __CONNECTIONWIDGET_H__
