#ifndef UDPCONNECTION_H
#define UDPCONNECTION_H

#include <QObject>
#include <QUdpSocket>
#include "trajectorypoint.h"

class UDPConnection : public QObject {
    Q_OBJECT
public:
    explicit UDPConnection(const QString& ip, quint16 port, QObject* parent = nullptr);

signals:
    void dataReceived(const QVector<TrajectoryPoint>& points);

private slots:
    void onReadyRead();

private:
    QUdpSocket* socket;
};

#endif // UDPCONNECTION_H
