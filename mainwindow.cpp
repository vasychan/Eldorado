#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qlistview.h>
 #include <QMetaType>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    , m_currentSong("empty")
{
    //init

    ui->setupUi(this);
    InitPlaylist();

    search_model = new QStandardItemModel();


    search_model->setHorizontalHeaderLabels(
        QStringList() << QApplication::translate("nestedlayouts", "Song")
                      << QApplication::translate("nestedlayouts", "Url "));


    //qDebug() << "hello from GUI thread " << QThread::currentThread();
    //connect button
    QObject::connect(ui->searchButton, SIGNAL(clicked()), this , SLOT(SearchSongHandler()));
    QObject::connect(ui->addButton, SIGNAL(clicked()), this , SLOT(AddSongHandler()));

    ui->searchView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    qRegisterMetaType<PlayStructList>("PlayStructList");
    connect(&playlist_thread, SIGNAL(UpdatePlaylist(PlayStructList)), this,
                                SLOT(ShowPlaylist(PlayStructList)));


    stream = new Stream;

    InitStream();
    //connect button stop stream
    QObject::connect(ui->pauseButton, SIGNAL(clicked()), this , SLOT(StartStopStream()));
    //ui->songNameLabel->setText("test");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SearchSongHandler()
{
    //qDebug() << ui->searchEdit->text();
    QString string = ui->searchEdit->text();
    boost::asio::io_service io_service;
    std::string utf_string = string.toUtf8().constData();
    std::string query = "/search?track="+utf_string+"&format=xml";
    HttpClient c(io_service, "mts.local", query);
    io_service.run();
    std::string response = c.getResponse();
    std::istringstream is(response);
    Parser *p = new Parser();
    SearchListList searchlist = p->ReadSearchlist(is);
    //QStringListModel *model = new QStringListModel();
    search_model->clear();
    QList<QStringList> rows;

    std::vector<SearchList>::iterator it;
    for ( it=searchlist.begin() ; it < searchlist.end(); it++ )
    {
        QStringList list;
        //std::cout << (*it).title << (*it).url << "\n";
        list << QString::fromUtf8((*it).title.c_str()) << (*it).url.c_str() << (*it).duration.c_str();
        rows.append(list);
    }
    foreach (QStringList row, rows) {
        QList<QStandardItem *> items;
        foreach (QString text, row)
            items.append(new QStandardItem(text));
        search_model->appendRow(items);
    }
    //search_model->setStringList(list);
    ui->searchView->setModel(search_model);
}

void MainWindow::AddSongHandler()
{
    //qDebug() << "AddSongHandler";
    //ui->searchView->selectedItem();

    QString name_song;
    QString url_song;
    QString duration_song;
    foreach(const QModelIndex &index, ui->searchView->selectionModel()->selectedIndexes())
    {
        QModelIndex url_song_index = index.sibling(index.row(),1);
        url_song = url_song_index.data().toString();
        QModelIndex duration_song_index = index.sibling(index.row(),2);
        duration_song = duration_song_index.data().toString();
        name_song = index.data().toString();
        //list.append(search_model->data(index));
        //list.append(search_model->itemFromIndex(index)->text());
    }
    if (name_song.size() == 0)
    {
        //qDebug() << "song name is empty";
        return;
    }

    boost::asio::io_service io_service;
    std::string utf_string = name_song.toUtf8().constData();
    std::string query = "/add?title="+utf_string+"&link="+url_song.toUtf8().constData()+"&duration="+duration_song.toUtf8().constData();
    //qDebug() << query.c_str();
    HttpClient c(io_service, "mts.local", query);
    io_service.run();

    //qDebug() << list.join(",");

}

void MainWindow::InitPlaylist()
{
    boost::asio::io_service io_service;
    HttpClient c(io_service, "mts.local", "/playlist");
    io_service.run();

    std::string t = c.getResponse();
    //std::cout << t << "\n";
    std::istringstream is(t);
    //QObject::connect(ui->addButton, SIGNAL(clicked()), this , SLOT(AddSongHandler()));
    Parser *p = new Parser();
    PlayStructList playlist = p->ReadPlaylist(is);
    //is.clear();

    playlist_model = new QStringListModel();
    QStringList list;


    std::vector<PlayStruct>::iterator it;
    for ( it=playlist.begin() ; it < playlist.end(); it++ )
    {
        //model->insertRow(it,(*it).song_name.c_str());
        list << QString::fromUtf8((*it).song_name.c_str());
    }
    playlist_model->setStringList(list);
    ui->playlistView->setModel(playlist_model);
}

void MainWindow::ShowPlaylist(PlayStructList playlist)
{
    //qDebug() << "TEST";

    QStringList list;
    std::vector<PlayStruct>::iterator it;
    int i = 1;
    std::string _song;
    for ( it=playlist.begin() ; it < playlist.end(); it++ )
    {
        //qDebug() << (*it).song_name.c_str();
           if(i == 1)
                _song = (*it).song_name;
        list << QString::fromUtf8((*it).song_name.c_str());
        i++;
    }
    bool reload = false;
    //check for new song
        qDebug() << m_currentSong.c_str();
if (m_currentSong != _song)
{
       reload = true;
}
    //MainWindow window;

    playlist_model->setStringList(list);

    //HACK!!!!
    qDebug() << stream->mediaObject->state();
    if (reload || stream->mediaObject->state() == 5)
    {
qDebug() << "RELOAD";
         stream->StopPlay();
         stream->playNow();
         if (stream->mediaObject->state() == 2 || stream->mediaObject->state() == 0  )
                m_currentSong = _song;
    }
}

void MainWindow::InitStream()
{
    QString string = "http://192.168.0.60:8000/test";
    //QString string = "http://voxsc1.somafm.com:2020";
    QUrl url(string);

    stream->SetUrl(url);
    stream->playNow();
    //QMap<QString, QString> metaData = stream->mediaObject->metaData();
    //QString title = metaData.value("TITLE");
    //qDebug() << title.toUtf8().constData();
    //ui->songNameLabel->setText(title);
}

void MainWindow::StartStopStream()
{

    if (stream->is_playing)
    {
        stream->StopPlay();
        QIcon icon = QIcon(":/icons/pause.png");
        ui->pauseButton->setIcon(icon);
    }
    else
    {
        stream->playNow();
        QIcon icon = QIcon(":/icons/play.png");
        ui->pauseButton->setIcon(icon);
        /*
        QMap<QString, QString> metaData = stream->mediaObject->metaData();
        //QString title = metaData.value("TITLE");
        QString title = metaData.value("ARTIST");
        if (title == "")
                 title = stream->mediaObject->currentSource().fileName();
        qDebug() << title.toUtf8().constData();
        ui->songNameLabel->setText(title);
        */
    }
}
