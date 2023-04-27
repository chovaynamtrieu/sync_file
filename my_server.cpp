#include "my_server.h"

MyServer::MyServer(QObject* parent) : QTcpServer(parent){}

void MyServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "New client connected!";

    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, [socket]() {
        QByteArray data = socket->readAll();
        qDebug() << "Received data:" << data;
    });

    connect(socket, &QTcpSocket::disconnected, this, [socket]() {
        qDebug() << "Client disconnected";
        socket->deleteLater();
    });

    // Add the new socket to the map
    sockets[0] = socket;
}

void MyServer::sendResponse(const QByteArray& data)
{
    QTcpSocket* socket = sockets[0];
    if (socket && socket->isValid() && socket->state() == QTcpSocket::ConnectedState) {
        socket->write(data);
    }
}
