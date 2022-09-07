#include "metadatadialog.h"
#include <QtWidgets>
#include <QFormLayout>
#include <QMediaMetaData>

MetaDataDialog::MetaDataDialog(QWidget *parent)
    :QDialog(parent)
{
    QFormLayout *mdLayout = new QFormLayout;
    for(int i=0; i<QMediaMetaData::NumMetaData; i++){
        QString label = QMediaMetaData::metaDataKeyToString(static_cast<QMediaMetaData::Key>(i));
        m_fields[i] = new QLineEdit;
        if(i == QMediaMetaData::ThumbnailImage){
            QPushButton *openThumbnail = new QPushButton(tr("Open"));
            connect(openThumbnail, &QPushButton::clicked, this, &MetaDataDialog::openThumbnailImage);
            QHBoxLayout *layout = new QHBoxLayout;
            layout->addWidget(m_fields[i]);
            layout->addWidget(openThumbnail);
            mdLayout->addRow(label, layout);
        }
        else if (i == QMediaMetaData::CoverArtImage) {
            QPushButton *openCoverArt = new QPushButton(tr("Open"));
            connect(openCoverArt, &QPushButton::clicked, this, &MetaDataDialog::openCoverArtImage);
            QHBoxLayout *layout = new QHBoxLayout;
            layout->addWidget(m_fields[i]);
            layout->addWidget(openCoverArt);
            mdLayout->addRow(label, layout);
        }
        else {
            if (i == QMediaMetaData::Title)
                m_fields[i]->setText(tr("Video test"));
            else if (i == QMediaMetaData::Author)
                m_fields[i]->setText(tr("Shura"));
            else if (i == QMediaMetaData::Date)
                m_fields[i]->setText(QDateTime::currentDateTime().toString());
            else if (i == QMediaMetaData::Date)
                m_fields[i]->setText(QDate::currentDate().toString());
            mdLayout->addRow(label, m_fields[i]);
        }
    }

    QWidget *viewPort = new QWidget;
    viewPort->setLayout(mdLayout);
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidget(viewPort);
    QVBoxLayout *dialLayout = new QVBoxLayout;
    this->setLayout(dialLayout);
    this->layout()->addWidget(scroll);

    auto bttnDialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);
    this->layout()->addWidget(bttnDialog);
    this->setWindowTitle("Metadata");
    this->resize(400, 300);

    connect(bttnDialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bttnDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

void MetaDataDialog::openThumbnailImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
        m_fields[QMediaMetaData::ThumbnailImage]->setText(fileName);
}

void MetaDataDialog::openCoverArtImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
        m_fields[QMediaMetaData::CoverArtImage]->setText(fileName);
}
