#include "settings.h"
#include "sqlite_db.h"
#include <QtWidgets>
#include <QFormLayout>

Settings::Settings(QWidget *parent)
            :QDialog(parent)
{
    QFormLayout *formLayout = new QFormLayout;
    /*подключение к БД*/
    sqlite_db_t db;
    db.dbname = "settings.s3db";
    if(db.open_db()){

        db.sql_str = "select * from params order by sort_id";
        if(db.exec_sql()){
            formLayout->setLabelAlignment(Qt::AlignRight);
            do{
                QString label = db.field_by_name("view_name").c_str();
                /*заголовки групп*/
                if(atoi(db.field_by_name("sort_id").c_str()) % 10 == 0){
                    SettWidget *settWidget = new SettWidget(new QLineEdit);
                    settWidget->m_id = db.field_by_name("sort_id").c_str();
                    settWidget->m_type = std::atoi(db.field_by_name("type").c_str());
                    settWidget->m_value = db.field_by_name("value").c_str();
                    m_fields.push_back(settWidget);
                    m_fields[m_fields.size()-1]->m_widget->setVisible(false);
                }
                else{
                    if(atoi(db.field_by_name("type").c_str()) == 4){
                        SettWidget *settWidget = new SettWidget(new QCheckBox);
                        settWidget->m_id = db.field_by_name("sort_id").c_str();
                        settWidget->m_type = std::atoi(db.field_by_name("type").c_str());
                        settWidget->m_value = db.field_by_name("value").c_str();
                        m_fields.push_back(settWidget);
                        if(sqlite_db_t::atob(db.field_by_name("value").c_str()))
                            static_cast<QCheckBox*>(m_fields[m_fields.size()-1]->m_widget)->setCheckState(Qt::CheckState::Checked);
                        else
                            static_cast<QCheckBox*>(m_fields[m_fields.size()-1]->m_widget)->setCheckState(Qt::CheckState::Unchecked);

                    }
                    else{
                        SettWidget *settWidget = new SettWidget(new QLineEdit);
                        settWidget->m_id = db.field_by_name("sort_id").c_str();
                        settWidget->m_type = std::atoi(db.field_by_name("type").c_str());
                        settWidget->m_value = db.field_by_name("value").c_str();
                        m_fields.push_back(settWidget);
                        static_cast<QLineEdit*>(m_fields[m_fields.size()-1]->m_widget)->setText(
                                db.field_by_name("value").c_str());
                    }
                }
                formLayout->addRow(label, m_fields[m_fields.size()-1]->m_widget);
            }while(db.next_rec());
        }
        else{
            QString label = "Error conect to DB";
            QLineEdit *ed = new QLineEdit;
            ed->setText("error SQL query");
            formLayout->addRow(label, ed);
        }
        db.close_db();
    }
    QWidget *viewPort = new QWidget;
    viewPort->setLayout(formLayout);
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidget(viewPort);
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    this->setLayout(dialogLayout);
    this->layout()->addWidget(scroll);
    auto bttnDialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);
    this->layout()->addWidget(bttnDialog);
    connect(bttnDialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bttnDialog, SIGNAL(rejected()), this, SLOT(reject()));
    this->setWindowTitle("Settings");
    this->resize(450, 300);
}
