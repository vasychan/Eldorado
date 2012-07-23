#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //init

    ui->setupUi(this);
    InitPlaylist();

    //init thread loop
    //Playlist playlist_loop;
    //playlist_loop.start();

    connect(&playlist_thread, SIGNAL(renderedImage(const QImage &, double)),
                 this, SLOT(updatePixmap(const QImage &, double)));


    qDebug() << "hello from GUI thread " << QThread::currentThread();
    //connect button
    QObject::connect(ui->searchButton, SIGNAL(clicked()), this , SLOT(SearchSongHandler()));
    QObject::connect(ui->addButton, SIGNAL(clicked()), this , SLOT(AddSongHandler()));

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
    QStringListModel *model = new QStringListModel();
    QStringList list;


    std::vector<SearchList>::iterator it;
    for ( it=searchlist.begin() ; it < searchlist.end(); it++ )
    {
        std::cout << (*it).title << "\n";
        list << (*it).title.c_str();
    }
    model->setStringList(list);
    ui->searchView->setModel(model);
}

void MainWindow::AddSongHandler()
{
    //ui->searchView->SelectedClicked();
}

void MainWindow::InitPlaylist()
{
    boost::asio::io_service io_service;
    HttpClient c(io_service, "mts.local", "/playlist");
    io_service.run();

    std::string t = c.getResponse();
    //std::cout << t << "\n";
    std::istringstream is(t);
    Parser *p = new Parser();
    PlayStructList playlist = p->ReadPlaylist(is);
    //is.clear();

    QStringListModel *model = new QStringListModel();
    QStringList list;


    std::vector<PlayStruct>::iterator it;
    for ( it=playlist.begin() ; it < playlist.end(); it++ )
    {
        list << (*it).song_name.c_str();
    }
    model->setStringList(list);
    ui->playlistView->setModel(model);
}
