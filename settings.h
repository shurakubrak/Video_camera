#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLineEdit>
#include <QVector>

/*Класс хранения настроек*/
class SettWidget
{
public:
    SettWidget(QWidget *widget){
        this->m_widget = widget;
    }
public:
    QWidget *m_widget;
    QString m_value;
    int m_type;
    QString m_id;
};

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    QVector<SettWidget*> m_fields;
};

#endif // SETTINGS_H
