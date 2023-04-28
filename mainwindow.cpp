#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , connection(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Chương trình đồng bộ dữ liệu");

    // Create a MyServer
    server = new MyServer;
    if (!server->listen(QHostAddress("127.0.0.1"), 1234)){
        qDebug() << "Failed to start server:" << server->errorString();
    }
    qDebug() << "Server started on IP" << server->serverAddress().toString() << "port" << server->serverPort();

    // Create a timer with a 30 second interval
    timer = new QTimer(this);
    connection = new QMetaObject::Connection;
    *connection = connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);

    // Connect the push button's clicked() signal to start the timer
    connect(ui->pushButton_syncFile, &QPushButton::clicked, this, [=](){
        timer->start(30000); // start the timer after the first click
    });
    connectionEstablished = true;

    //
    i = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete connection;
    delete server;
}


void MainWindow::on_pushButton_selectFile_clicked()
{
    //filePath = QFileDialog::getOpenFileName(this, tr("Open Text File"), QDir::currentPath(), tr("Text Files (*.txt)"));
    QFileDialog dialog(this, tr("Open Text File"), QDir::currentPath(), tr("Text Files (*.txt)"));
    dialog.setDirectory("D:/New_folder_(5)/repository/New_folder/New_folder_(4)/viettel-high-tech/bai_tap");

    filePath = dialog.getOpenFileName();

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->lineEdit_pathFile->setText(filePath);
            file.close();
        } else {
            qDebug() << "Failed to open file";
        }
    } else {
        qDebug() << "No file selected";
        ui->lineEdit_pathFile->setText("No file selected");
    }
}


void MainWindow::on_pushButton_syncFile_clicked()
{
    if (server->isClientConnected()){
        if (!connectionEstablished){
            *connection = connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
            connectionEstablished = true;
        }
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                in.setCodec("UTF-8");
                QString contents = in.readAll();
                QByteArray response1 = contents.toUtf8();
                server->sendResponse(response1);
                file.close();
                i +=1;
                qDebug() << "Send data:" << i;
            } else {
                qDebug() << "Failed to open file";
            }
        }
    }else{
        disconnect(*connection);
        connectionEstablished = false;
        qDebug() << "Stop callback.";
    }
}

void MainWindow::onTimerTimeout()
{
    on_pushButton_syncFile_clicked();
}

