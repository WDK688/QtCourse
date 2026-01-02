#include "chatserve.h"
#include "serverworker.h"

ChatServe::ChatServe(QObject *parent):
    QTcpServer(parent)
{

}

void ChatServe::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    connect(worker, &ServerWorker::logMessage,this, &ChatServe::logMessage);
    m_clients.append(worker);
    emit logMessage("新的用户连接上了");
}

void ChatServe::stopServer()
{
    close();
}
