#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new MyServer;
    if (!server->listen(QHostAddress("127.0.0.103"), 23)){
        qDebug() << "Failed to start server:" << server->errorString();
        //ui->~MainWindow();
    }
    qDebug() << "Server started on IP" << server->serverAddress().toString() << "port" << server->serverPort();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}


void MainWindow::on_pushButton_selectFile_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open Text File"), QDir::homePath(), tr("Text Files (*.txt)"));
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
    }
}


void MainWindow::on_pushButton_syncFile_clicked()
{
    //    std::ifstream file("D:\\New_folder_(5)\\repository\\New_folder\\New_folder_(4)\\viettel-high-tech\\bai_tap\\New Text Document.txt");
    //    if (!file.is_open()) {
    //        qDebug() << "Failed to open file";
    //    }

    //    std::stringstream buffer;
    //    buffer << file.rdbuf();
    //    std::string contents = buffer.str();
    //    file.close();

    //    QString data = ui->lineEdit_pathFile->text();


    //    QByteArray response1 = contents.c_str();
    //    QByteArray response2 = data.toUtf8();
    //    //QByteArray response = QByteArray("hello world!");
    //    server->sendResponse(response1);

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString contents = in.readAll();
            std::string contents2 = contents.toStdString();
            QByteArray response1 = contents2.c_str();
            server->sendResponse(response1);
            file.close();
        } else {
            qDebug() << "Failed to open file";
        }
    }
}

