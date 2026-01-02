#include "chatserve.h"
#include "serverworker.h"
#include<QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
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
    connect(worker, &ServerWorker::jsonReceived,this, &ChatServe::jsonReceived);
    connect(worker, &ServerWorker::disconnectdFromClient,this,std::bind( &ChatServe::userDisconnected,this,worker));
    m_clients.append(worker);
    emit logMessage("新的用户连接上了");
}

void ChatServe::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for(ServerWorker *worker :m_clients){
        worker->sendJson(message);
    }
}

void ChatServe::stopServer()
{
    close();
}

void ChatServe::jsonReceived(ServerWorker *sender, const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if (typeVal.isNull() || !typeVal.isString()) {
        return;
    }
    if (typeVal.toString().compare("message", Qt::CaseInsensitive) == 0) {
        const QJsonValue textVal = docObj.value("text");
        if (textVal.isNull() || !textVal.isString()) {
            return;
        }

        const QString text = textVal.toString().trimmed();

        if (text.isEmpty()) {
            return;
        }


        QJsonObject message;
        message["type"] = "message";       // 设置消息类型为"message"
        message["text"] = text;            // 设置消息文本内容
        message["sender"] = sender->userName(); // 设置消息发送者用户名

        // 广播该JSON消息（排除发送者自身）
        broadcast(message, sender);
    }else if (typeVal.toString().compare("login", Qt::CaseInsensitive) == 0) {

        const QJsonValue usernameVal = docObj.value("text");

        if (usernameVal.isNull() || !usernameVal.isString()) {
            return;
        }


        sender->setUserName(usernameVal.toString());

        QJsonObject connectedMessage;
        connectedMessage["type"] = "newuser";
        connectedMessage["username"] = usernameVal.toString();

        broadcast(connectedMessage, sender);
        QJsonObject userListMessage;
        userListMessage["type"] = "userlist";


        QJsonArray userlist;


        for (ServerWorker *worker : m_clients) {

            if (worker == sender) {
                userlist.append(worker->userName()+"*");
            } else {
                userlist.append(worker->userName());
            }
        }

        userListMessage["userlist"] = userlist;
        sender->sendJson(userListMessage);
    }
}

void ChatServe::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);

    const QString userName = sender->userName();

    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = "userdisconnected";
        disconnectedMessage["username"] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + " disconnected");
    }

    sender->deleteLater();
}
