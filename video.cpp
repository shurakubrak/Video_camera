#include "video.h"
#include "ui_video.h"
#include "settings.h"
#include "sqlite_db.h"
#include <QVideoWidget>
#include <QMediaDevices>
#include <QImageCapture>
#include <QMediaRecorder>
#include <QFileDialog>
#include <QCloseEvent>
#include <QCheckBox>
#include <QSqlDatabase>
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>

Video::Video(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Video)
{
    ui->setupUi(this);

    videoDevGr = new QActionGroup(this);
    videoDevGr->setExclusive(true);
    updateCamera();
    setCamera(QMediaDevices::defaultVideoInput());
}

Video::~Video()
{
    delete ui;
}

void Video::updateCamera()
{
    ui->menuDevices->clear();
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraDevice.description(), videoDevGr);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraDevice));
        if (cameraDevice == QMediaDevices::defaultVideoInput())
            videoDeviceAction->setChecked(true);

        ui->menuDevices->addAction(videoDeviceAction);
    }
}

void Video::setCamera(const QCameraDevice &cameraDevice)
{
    m_camera.reset(new QCamera(cameraDevice));
    m_captureSession.setCamera(m_camera.data());

    if(!m_mediaRecorder){
        m_mediaRecorder.reset(new QMediaRecorder);
        m_captureSession.setRecorder((m_mediaRecorder.data()));
//        connect(m_mediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this, &Camera::updateRecorderState);
    }

    m_imageCapture = new QImageCapture;
    m_captureSession.setImageCapture(m_imageCapture);

    m_captureSession.setVideoOutput(ui->video_wgt);

    connect(m_mediaRecorder.data(), SIGNAL(durationChanged(qint64)), this, SLOT(updateRecordTime(qint64)));
    connect(m_imageCapture, SIGNAL(readyForCaptureChanged(bool)), this, SLOT(readyForCapture(bool)));

    connect(ui->bttnSettings, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->takeImageButton, SIGNAL(clicked()), this, SLOT(takeImage()));
    connect(ui->bttnRecord, SIGNAL(clicked()), this, SLOT(record()));
    connect(ui->bttnPause, SIGNAL(clicked()), this, SLOT(pause()));
    connect(ui->bttnStop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->bttnMetadata, SIGNAL(clicked()), this, SLOT(mdShow()));
    connect(ui->bttnSettings, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->bttnPG, SIGNAL(clicked()), this, SLOT(showAnchors()));
    connect(ui->bttnPGW, SIGNAL(clicked()), this, SLOT(showTableW()));
    readyForCapture(m_imageCapture->isReadyForCapture());

    if(m_camera->cameraFormat().isNull()){
        auto formats = cameraDevice.videoFormats();
        if(!formats.isEmpty()){
            QCameraFormat bestFormat;
            for(const auto &fmt:formats){
                if(bestFormat.maxFrameRate() < 29
                        && fmt.maxFrameRate() > bestFormat.maxFrameRate())
                    bestFormat = fmt;
                else if(bestFormat.maxFrameRate() == fmt.maxFrameRate()
                        && bestFormat.resolution().width() * bestFormat.resolution().height()
                        < fmt.resolution().width() * fmt.resolution().height())
                    bestFormat = fmt;
            }
            m_camera->setCameraFormat(bestFormat);
            m_mediaRecorder->setVideoFrameRate(bestFormat.maxFrameRate());
        }
    }
    m_camera->start();
}

void Video::readyForCapture(bool ready)
{
    ui->takeImageButton->setEnabled(ready);
}

void Video::takeImage()
{
    QString strFileName = QFileDialog::getSaveFileName(this,tr("Save capture"),
                                                       "capture_1","JPG(*.jpg)");
    if(!strFileName.isNull())
    {
        m_isCapturingImage = true;
        m_imageCapture->captureToFile(strFileName);
    }
}

void Video::record()
{
    m_mediaRecorder->record();
    updateRecordTime(0);
}

void Video::updateRecordTime(qint64 tm)
{
    QString str = QString("Recorded %1 sec").arg(tm/1000);
    ui->statusbar->showMessage(str);
}

void Video::pause()
{
    m_mediaRecorder->pause();
}

void Video::stop()
{
    m_mediaRecorder->stop();
}

void Video::mdShow()
{
    if(!m_mdDialog)
        m_mdDialog = new MetaDataDialog(this);
    m_mdDialog->setAttribute(Qt::WA_DeleteOnClose, false);
    if(m_mdDialog->exec() == QDialog::Accepted)
        saveMetaData();
}

void Video::saveMetaData()
{
    QMediaMetaData data;
    for (int i = 0; i < QMediaMetaData::NumMetaData; i++) {
        QString val = m_mdDialog->m_fields[i]->text();
        if (!val.isEmpty()) {
            auto key = static_cast<QMediaMetaData::Key>(i);
            if (i == QMediaMetaData::CoverArtImage) {
                QImage coverArt(val);
                data.insert(key, coverArt);
            }
            else if (i == QMediaMetaData::ThumbnailImage) {
                QImage thumbnail(val);
                data.insert(key, thumbnail);
            }
            else if (i == QMediaMetaData::Date) {
                QDateTime date = QDateTime::fromString(val);
                data.insert(key, date);
            }
            else {
                data.insert(key, val);
            }
        }
    }
    m_mediaRecorder->setMetaData(data);
}

void Video::closeEvent(QCloseEvent *event)
{
    if (m_isCapturingImage) {
        setEnabled(false);
        event->ignore();
    } else {
        if(m_mediaRecorder->isAvailable())
            m_mediaRecorder->stop();
        if(m_camera->isActive())
            m_camera->stop();
        event->accept();
    }
}

void Video::showSettings()
{
    if(!m_Sett)
        m_Sett = new Settings(this);
    m_Sett->setAttribute(Qt::WA_DeleteOnClose, false);
    if(m_Sett->exec() == QDialog::Accepted)
        saveSettings();
}

void Video::showAnchors()
{
    if(!m_Anchors)
        m_Anchors = new dAnchors(this);
    m_Anchors->setAttribute(Qt::WA_DeleteOnClose, false);
    m_Anchors->show();
}

void Video::showTableW()
{
    if(!m_tableW)
        m_tableW = new dTable(this);
    m_tableW->setAttribute(Qt::WA_DeleteOnClose, false);
    m_tableW->show();
}

void Video::saveSettings()
{
    QString field;
    QVector<int> values;
    sqlite_db_t db;
    db.dbname = "settings.s3db";
    if(db.open_db()){
    for(auto it:m_Sett->m_fields){
        /*заголовок*/
        if(it->m_id[it->m_id.size()-1] == '0')
            continue;
        /*читаем поле*/
        if(it->m_type == 4){
            if(static_cast<QCheckBox*>(it->m_widget)->checkState() == Qt::CheckState::Checked)
                field = "Y";
            else
                field = "N";
        }
        else
            field = static_cast<QLineEdit*>(it->m_widget)->text();
        /*проверяем изменения*/
        if(field != it->m_value){
            db.sql_str = "update params set value = '"
                    + field.toStdString() + "' where sort_id="
                    + it->m_id.toStdString();
            db.exec_sql(true);
        }
    }
        db.close_db();
    }
}

