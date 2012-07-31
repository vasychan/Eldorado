#include "playlist.h"
#include "mainwindow.h"

Playlist::Playlist(QObject *parent)
: QThread(parent),
  quit(false)
{
    if (!isRunning())
        start();
    else
        cond.wakeOne();

}

Playlist::~Playlist()
{

    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();

}

void Playlist::run()
{

    while(!quit)
    {
        //qDebug() << "start from worker thread " << thread()->currentThreadId() ;
        sleep(1);
        boost::asio::io_service io_service;
        HttpClient c(io_service, "mts.local", "/playlist");
        io_service.run();

        std::string t = c.getResponse();
        std::istringstream is(t);
        Parser *p = new Parser();
        PlayStructList playlist = p->ReadPlaylist( is);
        //is.clear();
        //mutex.lock();
        emit UpdatePlaylist(playlist);
        //mutex.unlock();

        //qDebug() << "end from worker thread " << thread()->currentThreadId() ;

    }

}
