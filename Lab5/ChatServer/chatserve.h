#ifndef CHATSERVE_H
#define CHATSERVE_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"

class ChatServe : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServe(QObject *parent = nullptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
    QVector<ServerWorker *>m_clients;
    void broadcast(const QJsonObject &message, ServerWorker *exclude);

signals:
    void logMessage(const QString &msg);
public slots:
    void stopServer();
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void userDisconnected(ServerWorker *sender);

};
#endif // CHATSERVE_H
