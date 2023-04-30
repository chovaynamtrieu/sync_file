#include "my_server.h"

MyServer::MyServer(QObject* parent) : QTcpServer(parent){}

void MyServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "New client connected!";

    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &MyServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyServer::onDisconnected);

    // Add the new socket to the map
    sockets.insert(socketDescriptor, socket);

    QString ipAddress = socket->peerAddress().toString();
    qDebug() << "ip of the client: " << ipAddress;
}

void MyServer::onReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QByteArray data = socket->readAll();
        qDebug() << "Received data:" << data;
    }
}

// Remove all socket is UnconnectedState
void MyServer::onDisconnected()
{
    for (QMap<qintptr, QTcpSocket*>::iterator it = sockets.begin(); it != sockets.end();) {
        if (it.value()->state() == QAbstractSocket::UnconnectedState) {
            qDebug() << "Removing socket with descriptor" << it.key();
            it = sockets.erase(it);
        } else {
            ++it;
        }
    }
}

//Send data to all socket
void MyServer::sendResponse(const QByteArray& data)
{
    for (QTcpSocket* socket : sockets) {
        if (socket && socket->isValid() && socket->state() == QTcpSocket::ConnectedState) {
            socket->write(data);
        }
    }
}

// Check if the socket is still connected
bool MyServer::isClientConnected()
{
    for (QTcpSocket* socket : sockets) {
        if (socket && socket->isValid() && socket->state() == QTcpSocket::ConnectedState) {
            return true;
        }
    }
    qDebug() << "No Connection.";
    return false;
}
