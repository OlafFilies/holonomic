#ifndef SERIAL_H
#define SERIAL_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QException>
#include <QDebug>
#include <QObject>

class Serial : public QObject {
    Q_OBJECT
    QList<QSerialPortInfo> portList;
    QSerialPort* sPort;
    public:
    explicit Serial(QObject *parent = 0);
    Q_INVOKABLE QList<QString> getPInfo();
    Q_INVOKABLE QString selectPort(int i);
    Q_INVOKABLE void write(QString m);
    void readPort();
    void errorOccurred(QSerialPort::SerialPortError error);

    signals:
    void read(QString readContent);
    void serialError(QVariant errorMessage);

    public slots:

};

#endif // SERIAL_H
