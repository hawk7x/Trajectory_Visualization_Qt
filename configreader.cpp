#include "configreader.h"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QRegularExpression>

QRegularExpression ConfigReader::groupRegex(R"(\[(.*)\])");
QRegularExpression ConfigReader::keyValuePairRegex(R"(([^=]+)=(.*))");

ConfigReader::ConfigReader(const QString& filename) : connectionType(0), ip("127.0.0.1"), port(20000), filename_(filename) {
    QString filePath;
    QString appDirPath = QCoreApplication::applicationDirPath();
    QString sourceDirPath = QDir::currentPath();

    if (QFile::exists(QDir(appDirPath).filePath("../../../" + filename_))) {
        filePath = QDir(appDirPath).filePath("../../../" + filename_);
    } else if (QFile::exists(appDirPath + "/" + filename_)) {
        filePath = appDirPath + "/" + filename_;
    } else if (QFile::exists(sourceDirPath + "/" + filename_)) {
        filePath = sourceDirPath + "/" + filename_;
    } else {
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line;
        QString currentGroup;
        while (!in.atEnd()) {
            line = in.readLine().trimmed();
            QRegularExpressionMatch groupMatch = groupRegex.match(line);
            if (groupMatch.hasMatch()) {
                currentGroup = groupMatch.captured(1);
                continue;
            }
            QRegularExpressionMatch kvMatch = keyValuePairRegex.match(line);
            if (kvMatch.hasMatch()) {
                QString key = kvMatch.captured(1).trimmed();
                QString value = kvMatch.captured(2).trimmed();
                if (currentGroup == "General") {
                    if (key == "type_connect") {
                        bool ok;
                        connectionType = value.toInt(&ok);
                    } else if (key == "ip_number") {
                        ip = value;
                    } else if (key == "port_number") {
                        bool ok;
                        port = static_cast<quint16>(value.toUInt(&ok));
                    }
                }
            }
        }
        file.close();
    }
}

int ConfigReader::getConnectionType() const {
    return connectionType;
}

QString ConfigReader::getIp() const {
    return ip;
}

quint16 ConfigReader::getPort() const {
    return port;
}
