#ifndef QTABLEVIEWMODEL_H
#define QTABLEVIEWMODEL_H

#include "anchor.h"
#include <QAbstractListModel>
#include <QModelIndex>

class QTableViewModel : public QAbstractListModel
{
public:
    explicit QTableViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void populate(QList<Anchor> *newValues);

    void append(Anchor value);
    void update(int idx, Anchor value);
    void deleteRow(int idx);
    void insertAt(int idx, Anchor value);

private:
    QList<Anchor> *m_values;
};

#endif // QTABLEVIEWMODEL_H
