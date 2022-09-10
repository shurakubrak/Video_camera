#ifndef FANCHORS_H
#define FANCHORS_H

#include <QWidget>
#include "anchor.h"
#include "pgsql/include/libpq-fe.h"
#include "qtableviewmodel.h"
#include <QList>
#include "pgsql/include/libpq-fe.h"

namespace Ui {
class fAnchors;
}

class fAnchors : public QWidget
{
    Q_OBJECT

public:
    explicit fAnchors(QWidget *parent = nullptr);
    ~fAnchors();

private slots:
    void on_bttnAdd();
    void on_bttnUpdate();
    void on_bttnInsert();
    void on_bttnDelete();

private:
    Ui::fAnchors *m_ui;
    QList<Anchor> *m_lstAnchor;
    QTableViewModel *m_model;
    int m_newidx;
    PGconn *m_conn;

    PGconn* pgConnect(PGconn* conn);
    int pgExec(std::string qSQL, PGconn *conn, PGresult *res);
    void sqlFillData();
};

#endif // FANCHORS_H
