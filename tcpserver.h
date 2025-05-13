#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "trajectorypoint.h"

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(const QString& ip, quint16 port, QObject *parent = nullptr);
    ~TCPServer() override;

signals:
    void dataReceived(const QVector<TrajectoryPoint>& points);

private slots:
    void newConnection();
    void readyRead();
    void disconnected();

private:
    QTcpServer *server;
    QTcpSocket *clientSocket;
    QByteArray buffer;
};

#endif // TCPSERVER_H
