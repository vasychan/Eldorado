#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QThread>
#include "configure.h"
#include "lib/httpclient.h"
#include "lib/parser.h"

class Playlist : public QThread
{
    Q_OBJECT
public:
    Playlist(QObject *parent = 0);
    ~Playlist();

private:
    void run();
};

#endif // PLAYLIST_H
