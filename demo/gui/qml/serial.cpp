#include "serial.h"

Serial::Serial(QObject *parent) :QObject(parent){
    QList<QSerialPortInfo> portList;
    QSerialPort* sPort = NULL;
}

QList<QString> Serial::getPInfo(){
    portList = QSerialPortInfo::availablePorts();
    QList<QString> pInfo;
    for(int i = 0; i < portList.length(); i++){
        pInfo.append(portList[i].systemLocation());
    }
    return pInfo;
}

QString Serial::selectPort(int i){
    sPort = new QSerialPort(portList[i]);
    sPort->setBaudRate(QSerialPort::Baud38400);
    sPort->setDataBits(QSerialPort::Data8);
    sPort->setParity(QSerialPort::NoParity);
    sPort->setStopBits(QSerialPort::OneStop);
    sPort->setFlowControl(QSerialPort::NoFlowControl);
    sPort->open(QIODevice::ReadWrite);
    connect(sPort, &QSerialPort::readyRead, this, &Serial::readPort);
    connect(sPort, &QSerialPort::errorOccurred, this, &Serial::errorOccurred);
    emit serialError("");
    return portList[i].systemLocation();
}

void Serial::write(QString m){
    if(sPort != NULL){
        if(sPort->isOpen()){
            sPort->clear();
            sPort->write(m.toStdString().c_str(), m.toStdString().length());
        }
    } else{
        emit serialError("An error occurred while writing: Port not opened");
    }
}

void Serial::readPort(){
    QString va = sPort->readAll();
    qDebug() << va;
    emit read(va);
}

void Serial::errorOccurred(QSerialPort::SerialPortError error){
    qDebug() << error;
    emit serialError("An SP error occurred: " + error);
}
