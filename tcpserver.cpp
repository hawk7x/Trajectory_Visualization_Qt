#include "tcpserver.h"
#include <QHostAddress>
#include <QDebug>
#include <QDataStream>

TCPServer::TCPServer(const QString& ip, quint16 port, QObject *parent)
    : QObject(parent), server(new QTcpServer(this)), clientSocket(nullptr)
{
    QHostAddress address(ip);
    if (server->listen(address, port)) {
        qDebug() << "TCP Server listening on" << address.toString() << ":" << port;
        connect(server, &QTcpServer::newConnection, this, &TCPServer::newConnection);
    } else {
        qDebug() << "Error starting TCP server:" << server->errorString();
    }
}

TCPServer::~TCPServer()
{
    server->close();
}

void TCPServer::newConnection()
{
    clientSocket = server->nextPendingConnection();
    if (clientSocket) {
        qDebug() << "Client connected:" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
        connect(clientSocket, &QTcpSocket::readyRead, this, &TCPServer::readyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &TCPServer::disconnected);
    }
}

void TCPServer::readyRead()
{
    if (!clientSocket) return;
    buffer.append(clientSocket->readAll());

    while (buffer.size() >= 16) {
        if (buffer.left(4) != "GtoS") {
            qDebug() << "Invalid TCP packet header";
            buffer.remove(0, 1);
            continue;
        }

        QDataStream stream(buffer);
        stream.setByteOrder(QDataStream::LittleEndian);

        char mark[4];
        stream.readRawData(mark, 4);

        quint32 packetType;
        quint32 dataSize;
        qint32 pointCount;
        stream >> packetType >> dataSize >> pointCount;

        if (packetType != 456) {
            qDebug() << "Unknown TCP packet type:" << packetType;
            buffer.remove(0, 16);
            continue;
        }
        if (buffer.size() < 16 + pointCount * 8) {
            qDebug() << "Not enough TCP data in buffer";
            return;
        }
        QVector<TrajectoryPoint> points;
        const char* dataPtr = buffer.constData() + 16;
        for (int i = 0; i < pointCount; ++i) {
            float height = *reinterpret_cast<const float*>(dataPtr);
            dataPtr += sizeof(float);
            float distance = *reinterpret_cast<const float*>(dataPtr);
            dataPtr += sizeof(float);
            if (height >= 0) {
                points.append({height, distance});
                qDebug() << "TCP Point: Height=" << height << ", Distance=" << distance;
            }
        }
        emit dataReceived(points);
        buffer.remove(0, 16 + pointCount * 8);
    }
}

void TCPServer::disconnected()
{
    qDebug() << "Client disconnected:" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
    qDebug() << "";
    clientSocket->deleteLater();
    clientSocket = nullptr;
    buffer.clear();
}
