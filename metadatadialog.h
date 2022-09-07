#ifndef METADATADIALOG_H
#define METADATADIALOG_H

#include <QDialog>
#include <QMediaMetaData>
#include <QLineEdit>

class MetaDataDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MetaDataDialog(QWidget *parent = nullptr);
    QLineEdit *m_fields[QMediaMetaData::NumMetaData] = {};
private:
    void openThumbnailImage();
    void openCoverArtImage();
};

#endif // METADATADIALOG_H
