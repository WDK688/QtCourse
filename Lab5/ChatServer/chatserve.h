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

signals:
    void logMessage(const QString &msg);
public slots:
    void stopServer();

};
#endif // CHATSERVE_H
