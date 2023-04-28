#ifndef MYSERVER_H
#define MYSERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class MyServer : public QTcpServer
{
public:
    explicit MyServer(QObject* parent = nullptr);
    void sendResponse(const QByteArray& data);
    bool isClientConnected();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QMap<qintptr, QTcpSocket*> sockets;

};

#endif // MYSERVER_H
