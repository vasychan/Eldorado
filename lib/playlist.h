#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QThread>
#include "configure.h"
#include "lib/httpclient.h"
#include "lib/parser.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class Playlist : public QThread
{
    Q_OBJECT
public:
    Playlist(QObject *parent = 0);
    ~Playlist();


    void run();

signals:
    void UpdatePlaylist(PlayStructList playlist);

private:
    QMutex mutex;
    QWaitCondition cond;

    bool quit;

};

#endif // PLAYLIST_H
