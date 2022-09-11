#ifndef DTABLE_H
#define DTABLE_H

#include <QDialog>
#include "pgsql/include/libpq-fe.h"

namespace Ui {
class dTable;
}

class dTable : public QDialog
{
    Q_OBJECT

public:
    explicit dTable(QWidget *parent = nullptr);
    ~dTable();

private:
    Ui::dTable *ui;
    PGconn *m_conn;
    PGconn* pgConnect(PGconn* conn);
    void sqlFillData();
    void settTable(QStringList &headers);
private slots:
    void cellUpdate(int row, int col);
    void on_bttnAdd();
    void on_bttnDelete();
};

#endif // DTABLE_H
