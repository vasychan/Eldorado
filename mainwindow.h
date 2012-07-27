#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "lib/playlist.h"
#include "lib/stream.h"
#include "configure.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private:
    void InitPlaylist();
    void InitStream();


private:
    Ui::MainWindow *ui;
    Playlist playlist_thread;
    QStandardItemModel *search_model;
    QStringListModel *playlist_model;

    Phonon::MediaObject*   mediaObject;
    Phonon::AudioOutput*   audioOutput;

    Stream *stream;



public slots:
    void SearchSongHandler();
    void AddSongHandler();
    void ShowPlaylist(PlayStructList playlist);
    void StartStopStream();
};

#endif // MAINWINDOW_H
