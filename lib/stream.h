#ifndef STREAM_H
#define STREAM_H

#include "configure.h"

 #include <QWidget>
 #include <QList>
 #include <QLabel>
 #include <QPushButton>
 class QAction;
 class QTableWidget;
 class QLCDNumber;

  class Stream : public QWidget
 {
     Q_OBJECT
 public:
     Stream();
     QSize sizeHint() const {
         return QSize(500, 300);
     }

     Phonon::MediaObject*   mediaObject;
     Phonon::AudioOutput*   audioOutput;
     Phonon::MediaSource source;
     QLabel*                mStatusLabel;
     QLabel*                mStatusTitle;
     QAction*               playAction;
     QAction*               pauseAction;
     QAction*               stopAction;
     QLCDNumber*            timeLcd;
     Phonon::VolumeSlider*  volumeSlider;
     QWidget*               dummy;

     bool is_playing;
 public slots:
     void playNow();
     void StopPlay();
     void SetFile(const QString& file);
     void SetUrl(const QUrl &url);
     void stateChanged(Phonon::State newState, Phonon::State oldState);
     void sourceChanged(const Phonon::MediaSource &source);
     void aboutToFinish();
     void setBufferingValue(int value);
     void tick(qint64 time);
signals:
     void backButtonPressed();
     void UpdateTimer(QTime time);
private:
     void setupUi();
private:


 };
#endif // STREAM_H
