#ifndef MYSERVER_H
#define MYSERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class MyServer : public QTcpServer
{
public:
    explicit MyServer(QObject* parent = nullptr);
    void sendResponse(const QByteArray& data);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    QMap<int, QTcpSocket*> sockets;
};

#endif // MYSERVER_H
