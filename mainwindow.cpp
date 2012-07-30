#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qlistview.h>
 #include <QMetaType>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //init

    ui->setupUi(this);
    InitPlaylist();

    search_model = new QStandardItemModel();


    search_model->setHorizontalHeaderLabels(
        QStringList() << QApplication::translate("nestedlayouts", "Song")
                      << QApplication::translate("nestedlayouts", "Url "));


    qDebug() << "hello from GUI thread " << QThread::currentThread();
    //connect button
    QObject::connect(ui->searchButton, SIGNAL(clicked()), this , SLOT(SearchSongHandler()));
    QObject::connect(ui->addButton, SIGNAL(clicked()), this , SLOT(AddSongHandler()));

    ui->searchView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    qRegisterMetaType<PlayStructList>("PlayStructList");
    connect(&playlist_thread, SIGNAL(UpdatePlaylist(PlayStructList)), this,
                                SLOT(ShowPlaylist(PlayStructList)));


    //stream = new Stream;
    stream = new g_stream;

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
    qDebug() << ui->searchEdit->text();
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
        std::cout << (*it).title << (*it).url << "\n";
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
    qDebug() << "AddSongHandler";
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
        qDebug() << "song name is empty";
        return;
    }

    boost::asio::io_service io_service;
    std::string utf_string = name_song.toUtf8().constData();
    std::string query = "/add?title="+utf_string+"&link="+url_song.toUtf8().constData()+"&duration="+duration_song.toUtf8().constData();
    qDebug() << query.c_str();
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
    qDebug() << "TEST";
    QStringList list;
    std::vector<PlayStruct>::iterator it;
    for ( it=playlist.begin() ; it < playlist.end(); it++ )
    {
        //qDebug() << (*it).song_name.c_str();
        list << QString::fromUtf8((*it).song_name.c_str());
    }

    //MainWindow window;

    playlist_model->setStringList(list);

    //HACK!!!!
    if (stream->is_playing == true)
    {
         stream->play();
    }
}

void MainWindow::InitStream()
{
    /*
    gst_init (NULL,NULL);
     // Record pipeline
     // autoaudiosrc ! audioconvert ! lame bitrate=192 ! filesink location=./ciao.mp3
     GstElement *recordBin = gst_pipeline_new("record-pipeline");
     g_assert(recordBin);
     GstElement *audioSrc = gst_element_factory_make("autoaudiosrc", "audio_in");
     g_assert(audioSrc);
     GstElement *audioConvert = gst_element_factory_make("audioconvert", "audio_converter");
     g_assert(audioConvert);
     GstElement *lameEncoder = gst_element_factory_make("lame", "lame_encoder");
     g_assert(audioConvert);
     GstElement *fileSink = gst_element_factory_make("filesink", "file_sink");
     g_assert(fileSink);
     gst_bin_add_many(GST_BIN(recordBin), audioSrc, audioConvert, lameEncoder, fileSink, NULL);
     gst_element_link_many(audioSrc, audioConvert, lameEncoder, fileSink, NULL);
     // Playback
     GstElement *playbackBin = gst_element_factory_make("playbin2", "playbin");
     g_assert(playbackBin);
     QString uri = "http://192.168.0.60:8000/test";
    qDebug() << uri;
     g_object_set(G_OBJECT(playbackBin), "uri", qPrintable(uri), NULL);
     gst_element_set_state(GST_ELEMENT(recordBin), GST_STATE_READY);
     gst_element_set_state(GST_ELEMENT(playbackBin), GST_STATE_PLAYING);
    */
    QString uri = "http://192.168.0.60:8000/test";
    //QString string = "http://voxsc1.somafm.com:2020";

    stream->SetUrl(uri);
    stream->play();
    //QMap<QString, QString> metaData = stream->mediaObject->metaData();
    //QString title = metaData.value("TITLE");
    //qDebug() << title.toUtf8().constData();
    //ui->songNameLabel->setText(title);
}

void MainWindow::StartStopStream()
{
    if (stream->is_playing)
    {
        stream->stop();
        QIcon icon = QIcon(":/icons/pause.png");
        ui->pauseButton->setIcon(icon);
    }
    else
    {
        stream->play();
        QIcon icon = QIcon(":/icons/play.png");
        ui->pauseButton->setIcon(icon);
    }
}
