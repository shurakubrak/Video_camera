#include "danchors.h"
#include "ui_danchors.h"

dAnchors::dAnchors(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::dAnchors)
{
    m_ui->setupUi(this);
    /*Postres*/
    m_conn = nullptr;
    m_conn = pgConnect(m_conn);
    if(m_conn == nullptr)
        qDebug() << "Postgres connect Ok";
    else
        sqlFillData();
}

dAnchors::~dAnchors()
{
    delete m_ui;
}

void dAnchors::on_bttnAdd()
{
    m_newidx++;
    QString strIdx = QString().number(m_newidx);
    m_model->append(Anchor(m_newidx,"127.0.0.1", "", "127.0.0.1", 5555));
}

void dAnchors::on_bttnUpdate()
{
    m_model->update(m_ui->table->currentIndex().row(), Anchor(0, "127.0.0.1", "", "127.0.0.1", 5555));
}

void dAnchors::on_bttnInsert()
{
    m_model->insertAt(0, Anchor(0, "127.0.0.1", "", "127.0.0.1", 5555));
}

void dAnchors::on_bttnDelete()
{
    m_model->deleteRow(0); //почему '0'
}


PGconn* dAnchors::pgConnect(PGconn* conn)
{
    std::string pghost = "172.16.26.215";
    std::string pgport = "5432";
    std::string pgoptions = "";
    std::string pgtty = "";
    std::string dbName = "rtls";
    std::string login = "postgres";
    std::string pwd = "pgadmin";	//"pgadmin";

    conn = PQsetdbLogin(pghost.c_str(),
                pgport.c_str(),
                pgoptions.c_str(),
                pgtty.c_str(),
                dbName.c_str(),
                login.c_str(),
                pwd.c_str());

    if (PQstatus(conn) != CONNECTION_OK && PQsetnonblocking(conn, 1) != 0) {
        std::string err = PQerrorMessage(conn);
        qDebug() << "PostgreSQL eroor: " << err.c_str();
            return nullptr;
    }
    return conn;
}

int dAnchors::pgExec(std::string qSQL, PGconn *conn, PGresult *res)
{
    if (qSQL.empty())
        return -1;

    if (PQstatus(conn) == CONNECTION_OK) {
        res = PQexec(conn, qSQL.c_str());
        switch (PQresultStatus(res))
        {
        case PGRES_COMMAND_OK:
            PQclear(res);
            return PGRES_COMMAND_OK;
        case PGRES_TUPLES_OK:
            return PGRES_TUPLES_OK;
        case PGRES_FATAL_ERROR:
            qDebug() << "ERR: Query fail";
            PQclear(res);
            return PGRES_FATAL_ERROR;
        default:
            PQclear(res);
            break;
        }
    }
    return -1;
}

void dAnchors::sqlFillData()
{
    m_lstAnchor = new QList<Anchor>;
    std::string query = "select * from s_anchors";

    if(PQstatus(m_conn) == CONNECTION_OK){
        PGresult *res = PQexec(m_conn, query.c_str());
        if(PQresultStatus(res) == PGRES_TUPLES_OK){
            size_t nRows = PQntuples(res);
            for(size_t i=0; i<nRows; i++){
                m_lstAnchor->push_back(Anchor(atoi(PQgetvalue(res,i, 0)),
                                          PQgetvalue(res,i, 1),
                                          PQgetvalue(res,i, 2),
                                          PQgetvalue(res,i, 3),
                                          atoi(PQgetvalue(res,i, 4))));
        }
        m_model = new QTableViewModel;
        m_model->populate(m_lstAnchor);
        m_ui->table->setModel(m_model);
        m_newidx = 0;
    }
    }
}
