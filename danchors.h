#ifndef DANCHORS_H
#define DANCHORS_H

#include <QDialog>
#include "anchor.h"
#include "pgsql/include/libpq-fe.h"
#include "qtableviewmodel.h"
#include <QList>
#include "pgsql/include/libpq-fe.h"

namespace Ui {
class dAnchors;
}

class dAnchors : public QDialog
{
    Q_OBJECT

public:
    explicit dAnchors(QWidget *parent = nullptr);
    ~dAnchors();

private slots:
    void on_bttnAdd();
    void on_bttnUpdate();
    void on_bttnInsert();
    void on_bttnDelete();

private:
    Ui::dAnchors *m_ui;
    QList<Anchor> *m_lstAnchor;
    QTableViewModel *m_model;
    int m_newidx;
    PGconn *m_conn;

    PGconn* pgConnect(PGconn* conn);
    int pgExec(std::string qSQL, PGconn *conn, PGresult *res);
    void sqlFillData();
};

#endif // DANCHORS_H
