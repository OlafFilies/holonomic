#include "qbserver.h"

void QBServer::startServer()
{
    setMessage(QStringLiteral("Starting the server"));
    m_serverInfo = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_serverInfo, &QBluetoothServer::newConnection,
            this, &PingPong::clientConnected);
    connect(m_serverInfo, QOverload<QBluetoothServer::Error>::of(&QBluetoothServer::error),
            this, &PingPong::serverError);
    const QBluetoothUuid uuid(serviceUuid);

    m_serverInfo->listen(uuid, QStringLiteral("PingPong server"));
    setMessage(QStringLiteral("Server started, waiting for the client. You are the left player."));
}
