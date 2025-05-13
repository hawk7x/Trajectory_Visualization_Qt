#include "udpconnection.h"
#include <QDataStream>
#include <QDebug>

UDPConnection::UDPConnection(const QString& ip, quint16 port, QObject* parent)
    : QObject(parent), socket(new QUdpSocket(this)) {
    QHostAddress address(ip);
    if (socket->bind(address, port)) {
        qDebug() << "UDP Socket successfully bound to" << address.toString() << ":" << port;
        connect(socket, &QUdpSocket::readyRead, this, &UDPConnection::onReadyRead);
    } else {
        qDebug() << "Error binding UDP socket to" << address.toString() << ":" << port << "-" << socket->errorString();
    }
}

void UDPConnection::onReadyRead() {
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(socket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QDataStream stream(datagram);
        stream.setByteOrder(QDataStream::LittleEndian);
        char mark[4];
        stream.readRawData(mark, 4);
        if (strncmp(mark, "GloS", 4) != 0) {
            qDebug() << "Invalid UDP packet header:" << QByteArray(mark).toHex();
            continue;
        }
        quint32 packetType;
        quint32 dataSize;
        qint32 pointCount;
        stream >> packetType >> dataSize >> pointCount;
        if (packetType != 456) {
            qDebug() << "Unknown UDP packet type:" << packetType;
            continue;
        }
        if (datagram.size() < 16 + pointCount * 8) {
            qDebug() << "Incomplete UDP data, expected" << 16 + pointCount * 8 << "bytes, got" << datagram.size();
            continue;
        }
        QVector<TrajectoryPoint> points;
        const char* dataPtr = datagram.constData() + 16;
        for (int i = 0; i < pointCount; ++i) {
            float height = *reinterpret_cast<const float*>(dataPtr);
            dataPtr += sizeof(float);
            float distance = *reinterpret_cast<const float*>(dataPtr);
            dataPtr += sizeof(float);
            if (height >= 0) {
                points.append({height, distance});
                qDebug() << "UDP Point: Height=" << height << ", Distance=" << distance;
            }
        }
        qDebug() << "";
        emit dataReceived(points);
    }
}
