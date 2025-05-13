#include <QApplication>
#include "mainwindow.h"
#include "configreader.h"
#include "tcpserver.h"
#include "udpconnection.h"
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ConfigReader config("config.ini");
    MainWindow window;
    window.show();

    int connectionType = config.getConnectionType();
    if (connectionType == 1) {
        qDebug() << "Creating TCP server";
        TCPServer *tcpServer = new TCPServer(config.getIp(), config.getPort(), &window);
        QObject::connect(tcpServer, &TCPServer::dataReceived, &window, &MainWindow::updatePlot);
    } else if (connectionType == 2) {
        qDebug() << "Creating UDP connection";
        UDPConnection *udp = new UDPConnection(config.getIp(), config.getPort(), &window);
        QObject::connect(udp, &UDPConnection::dataReceived, &window, &MainWindow::updatePlot);
    } else {
        qDebug() << "Unknown connection type in config file: " << connectionType;
    }

    return app.exec();
}
