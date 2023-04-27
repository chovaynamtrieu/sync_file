#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "my_server.h"
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

private:
    Ui::MainWindow *ui;
    MyServer *server;
    QString filePath;
    int i; //so lan gui data
};
#endif // MAINWINDOW_H
