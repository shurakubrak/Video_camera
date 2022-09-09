#ifndef VIDEO_H
#define VIDEO_H

#include <QMainWindow>
#include <QCamera>
#include <QActionGroup>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include "metadatadialog.h"
#include "settings.h"
#include "pgsql/include/libpq-fe.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Video; }
QT_END_NAMESPACE

class Video : public QMainWindow
{
    Q_OBJECT

public:
    Video(QWidget *parent = nullptr);
    ~Video();

private:
    Ui::Video *ui;

    QActionGroup *videoDevGr = nullptr;
    QScopedPointer<QCamera> m_camera;
    QMediaCaptureSession m_captureSession;
    QScopedPointer<QMediaRecorder> m_mediaRecorder;
    QImageCapture *m_imageCapture;

    bool m_isCapturingImage = false;
    MetaDataDialog *m_mdDialog = nullptr;
    Settings *m_Sett = nullptr;

private slots:
    void takeImage();
    void updateCamera();
    void setCamera(const QCameraDevice &cameraDevice);
    void readyForCapture(bool ready);
    void record();
    void updateRecordTime(qint64 tm);
    void pause();
    void stop();
    void mdShow();
    void saveMetaData();
    void showSettings();
    void sqlOpen();
protected:
    void closeEvent(QCloseEvent *event) override;
    void saveSettings();
    PGconn* pgConnect(PGconn* conn);
    int pgExec(std::string qSQL, PGconn *conn);
};
#endif // VIDEO_H
