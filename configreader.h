#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QString>
#include <QRegularExpression>

class ConfigReader {
public:
    ConfigReader(const QString& filename);
    int getConnectionType() const; // 1 - TCP, 2 - UDP
    QString getIp() const;
    quint16 getPort() const;

private:
    int connectionType;
    QString ip;
    quint16 port;
    QString filename_;
    static QRegularExpression groupRegex;
    static QRegularExpression keyValuePairRegex;
};

#endif // CONFIGREADER_H
