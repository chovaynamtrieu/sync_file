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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , connection(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Chương trình đồng bộ dữ liệu");

    // Create a MyServer
    get_directory();
    server = new MyServer;
    host = "127.0.0.1";
    port = 1234;
    if (!server->listen(QHostAddress(host), port)){
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
    save_directory();
    delete ui;
    delete timer;
    delete connection;
    delete server;
}


void MainWindow::on_pushButton_selectFile_clicked()
{
    if (directory == ""){
        filePath = QFileDialog::getOpenFileName(this, tr("Open Text File"), QDir::currentPath(), tr("Text Files (*.txt);;All File (*)"));
    }else{
        filePath =  QFileDialog::getOpenFileName(this, tr("Open Text File"), directory, tr("Text Files (*.txt);;All File (*)"));
    }

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        QFileInfo fileInfo(filePath);
        directory = fileInfo.dir().absolutePath();
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
    //checks if the client is connected to the server
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
                // The separator between the file name and data payload is assumed to be "\r\n".
                QByteArray response2 = contents.toUtf8();
                QString fileName = QFileInfo(filePath).fileName();       // get the name of the file
                QByteArray response1 = fileName.toUtf8();                // convert the file name to a QByteArray
                QByteArray separator = QByteArrayLiteral("\r\n");        // add a separator between the data and file name
                QByteArray response = response1 + separator + response2; // combine the data and file name
                server->sendResponse(response);
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

void MainWindow::get_directory(){
    //QFile file("D:\\New_folder_(5)\\repository\\New_folder\\New_folder_(4)\\viettel-high-tech\\bai_tap\\bai_tap\\history.txt");
    QFile file("history.txt");
    QString contents;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        contents = in.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());
        if (!doc.isNull()) {
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
//                host = obj["host"].toString();
//                port = obj["port"].toInt();
                directory = obj["directory"].toString();
                qDebug() << "Host:" << host << " port:" << port << " directory:" << directory;
            }
        }
    }
}

void MainWindow::save_directory(){
    QFile file("history.txt");
    QString contents;
    QString host_temp = "", directory_temp = "";
    int port_temp = 0;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        contents = in.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());
        if (!doc.isNull()) {
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                host_temp = obj["host"].toString();
                port_temp = obj["port"].toInt();
                directory_temp = obj["directory"].toString();
            }
        }
    }
    QString res;
    if (host_temp != host || port_temp != port || directory_temp != directory){
        res = "{\"host\":\""+host+"\", \"port\":"+QString::number(port)+", \"directory\":\""+directory+"\"}";
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << res;
            file.close();
        }
    }
}

