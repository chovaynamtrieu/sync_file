#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "my_server.h"
#include <iostream>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onTimerTimeout();

private slots:
    void on_pushButton_selectFile_clicked();

    void on_pushButton_syncFile_clicked();

protected:
    void get_directory();  // retrieves a directory path from history
    void save_directory(); // saves a directory path to history

private:
    Ui::MainWindow *ui;

    MyServer *server;
    QString host; // the IP address
    int port;     // that stores the port number that the server should listen on

    QString filePath;  // the path to a file
    QString directory; // This variable stores the path to the directory where the last selected file is located


    bool connectionEstablished;
    QMetaObject::Connection *connection; // connection to timer
    QTimer *timer; // that is used to trigger events at regular intervals
    int i;         // count the number of times data is sent
};
#endif // MAINWINDOW_H
