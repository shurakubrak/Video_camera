#include "dtable.h"
#include "ui_dtable.h"

dTable::dTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dTable)
{
    ui->setupUi(this);
    m_conn = pgConnect(m_conn);
    if(m_conn == nullptr)
        qDebug() << "Postgres connect Ok";
    else{
        settTable(QStringList() << "ID"
                  << "ip_addr"
                  << "descr"
                  << "server_addr"
                  << "server_port");
        sqlFillData();
    }
}

dTable::~dTable()
{
    delete ui;
}

void dTable::settTable(QStringList &headers)
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setShowGrid(true);
    /* Разрешаем выделение только одного элемента */
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    /* Разрешаем выделение построчно */
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* Устанавливаем заголовки колонок */
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    /* Растягиваем последнюю колонку */
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    /* Скрываем колонку под номером 0 (id) */
    ui->tableWidget->hideColumn(0);
}

PGconn* dTable::pgConnect(PGconn* conn)
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

void dTable::sqlFillData()
{
    std::string query = "select * from s_anchors";
    if(PQstatus(m_conn) == CONNECTION_OK){
        PGresult *res = PQexec(m_conn, query.c_str());
        if(PQresultStatus(res) == PGRES_TUPLES_OK){
            size_t nRows = PQntuples(res);
            for(size_t i=0; i<nRows; i++){
                /* Вставляем строку */
                ui->tableWidget->insertRow(i);
                /* заполняем поля */
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(PQgetvalue(res, i, 0)));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(PQgetvalue(res, i, 1)));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(PQgetvalue(res, i, 2)));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(PQgetvalue(res, i, 3)));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(PQgetvalue(res, i, 4)));
            }
        }
    }
}
