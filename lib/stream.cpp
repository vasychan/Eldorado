#include "stream.h"

#include <QtGui>

 Stream::Stream()
     : is_playing(false)
 {
     audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
     mediaObject = new Phonon::MediaObject(this);

     mediaObject->setTickInterval(1000);
     connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
     connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),this, SLOT(stateChanged(Phonon::State,Phonon::State)));
     connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),this, SLOT(sourceChanged(Phonon::MediaSource)));
     connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

     connect(mediaObject, SIGNAL(bufferStatus(int)), this, SLOT(setBufferingValue(int)));

     Phonon::createPath(mediaObject, audioOutput);

     playAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
     playAction->setDisabled(true);

     pauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
     pauseAction->setDisabled(true);

     stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
     stopAction->setDisabled(true);

     connect(playAction, SIGNAL(triggered()), mediaObject, SLOT(play()));
     connect(pauseAction, SIGNAL(triggered()), mediaObject, SLOT(pause()) );
     connect(stopAction, SIGNAL(triggered()), mediaObject, SLOT(stop()));

     setupUi();
 }
 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::SetFile(const QString& file)
 {
    source = Phonon::MediaSource(file);
 }
 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::StopPlay()
 {
    mediaObject->stop();
    mediaObject->clearQueue();
    this->is_playing = false;
 }
 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::SetUrl(const QUrl &url)
 {
    source = Phonon::MediaSource(url);
 }

 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::playNow()
 {
    mediaObject->stop();
    mediaObject->clearQueue();

    mediaObject->setCurrentSource(source);
    mediaObject->play();
    this->is_playing = true;
 }

 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::stateChanged(Phonon::State newState, Phonon::State /* oldState */)
 {
     switch (newState) {
         case Phonon::ErrorState:
             if(mediaObject){
                 if (mediaObject->errorType() == Phonon::FatalError) {
                     QMessageBox::warning(this, tr("Fatal Error"),
                     mediaObject->errorString());
                 } else {
                    QMessageBox::warning(this, tr("Error"),
                    mediaObject->errorString());
                 }
             }else{
                 QMessageBox::warning(this, tr("Error"),tr("Media is NULL"));
             }
             break;
         case Phonon::PlayingState:
                 mStatusLabel->setText("PlayingState");
                 playAction->setEnabled(false);
                 pauseAction->setEnabled(true);
                 stopAction->setEnabled(true);
                 break;
         case Phonon::StoppedState:
                 mStatusLabel->setText("StoppedState");
                 stopAction->setEnabled(false);
                 playAction->setEnabled(true);
                 pauseAction->setEnabled(false);
                 timeLcd->display("00:00");
                 break;
         case Phonon::PausedState:
                 mStatusLabel->setText("PausedState");
                 pauseAction->setEnabled(false);
                 stopAction->setEnabled(true);
                 playAction->setEnabled(true);
                 break;
         case Phonon::BufferingState:
                 mStatusLabel->setText("BufferingState");
                 break;
         default:
             break;
     }
 }

 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::setBufferingValue(int value)
{
     QString statt = "Buffering: " + QString::number(value);
     mStatusLabel->setText(statt);
}

 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::sourceChanged(const Phonon::MediaSource& /*source*/)
 {
    mStatusLabel->setText("sourceChanged");
    timeLcd->display("00:00");
 }
 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::tick(qint64 time)
 {
     QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

     timeLcd->display(displayTime.toString("mm:ss"));
 }

 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::aboutToFinish()
 {
     emit backButtonPressed();
 }

 /*
  * --------------------------------------------------------------
  * --------------------------------------------------------------
  */
 void Stream::setupUi()
 {
     mStatusTitle = new QLabel(tr("Status:"));
     mStatusLabel = new QLabel();

     dummy = new QWidget();
     dummy->setDisabled(true);

     QHBoxLayout *topLayout1 = new QHBoxLayout;
     topLayout1->addWidget(mStatusTitle);
     topLayout1->addWidget(mStatusLabel);

     QToolBar *bar = new QToolBar;
     bar->addAction(playAction);
     bar->addAction(pauseAction);
     bar->addAction(stopAction);

     QPalette palette;
     palette.setBrush(QPalette::Light, Qt::darkGray);
     timeLcd = new QLCDNumber;
     timeLcd->setPalette(palette);

     volumeSlider = new Phonon::VolumeSlider(this);
     volumeSlider->setAudioOutput(audioOutput);
     volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addLayout(topLayout1);
     mainLayout->addWidget(bar);
     mainLayout->addWidget(volumeSlider);
     mainLayout->addWidget(timeLcd);
     setLayout(mainLayout);
 }
