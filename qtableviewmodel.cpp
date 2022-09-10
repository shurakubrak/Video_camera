#include "qtableviewmodel.h"

QTableViewModel::QTableViewModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_values = new QList<Anchor>;
}

int QTableViewModel::rowCount(const QModelIndex &) const
{
    return m_values->count();
}

int QTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 5;   /* количество полей класса Anchor */
}

QVariant QTableViewModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            m_values->at(index.row()).getAnchorId();
            break;
        case 1:
            m_values->at(index.row()).getIpAddr();
            break;
        case 2:
            m_values->at(index.row()).getDescription();
            break;
        case 3:
            m_values->at(index.row()).getServerIp();
            break;
        case 4:
            m_values->at(index.row()).getServerPort();
            break;
        }
        break;
    case Qt::UserRole:
        value = m_values->at(index.row()).getAnchorId();
        break;
    }
    return value;
}

QVariant QTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QString("ID");
        case 1:
            return QString("IP addr");
        case 2:
            return QString("Description");
        case 3:
            return QString("server IP addr");
        case 4:
            return QString("server port");
        }
    }
    return QVariant();
}

void QTableViewModel::populate(QList<Anchor> *newValues)
{
    int idx = m_values->count();
    this->beginInsertRows(QModelIndex(), 1, idx);
    m_values = newValues;
    this->endInsertRows();
}

void QTableViewModel::append(Anchor value)
{
    int newRow = m_values->count() + 1;
    this->beginInsertRows(QModelIndex(), newRow, newRow);
    m_values->append(value);
    this->endInsertRows();
}

void QTableViewModel::update(int idx, Anchor value)
{
    (*m_values)[idx] = value;
    QModelIndex index_s = this->index(idx, 0);
    QModelIndex index_e = this->index(idx, this->columnCount(QModelIndex()));
    emit this->dataChanged(index_s, index_e);
}

void QTableViewModel::deleteRow(int idx)
{
    this->beginRemoveRows(QModelIndex(), idx, idx);
    (*m_values).removeAt(idx);
    this->endRemoveRows();
}

void QTableViewModel::insertAt(int idx, Anchor value)
{
    int newRow = idx;
    this->beginInsertRows(QModelIndex(), newRow, newRow);
    m_values->insert(newRow, value);
    this->endInsertRows();
}
