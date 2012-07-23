#include "playlist.h"

Playlist::Playlist(QObject *parent)
: QThread(parent)
{
}

Playlist::~Playlist()
{

}

void Playlist::run()
{
    while(true)
    {
        sleep(3);
        boost::asio::io_service io_service;
        HttpClient c(io_service, "mts.local", "/playlist");
        io_service.run();

        std::string t = c.getResponse();
        //std::cout << t << "\n";
        std::istringstream is(t);
        Parser *p = new Parser();
        PlayStructList playlist = p->ReadPlaylist( is);
        //is.clear();

        QStringListModel *model = new QStringListModel();
        QStringList list;


        std::vector<PlayStruct>::iterator it;
        for ( it=playlist.begin() ; it < playlist.end(); it++ )
        {
            list << (*it).song_name.c_str();
        }
        model->setStringList(list);

        qDebug() << "hello from worker thread " << thread()->currentThreadId() ;

    }

}
