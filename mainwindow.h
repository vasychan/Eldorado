#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "lib/playlist.h"
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
private:
    Ui::MainWindow *ui;
    Playlist playlist_thread;

public slots:
    void SearchSongHandler();
    void AddSongHandler();
};

#endif // MAINWINDOW_H
