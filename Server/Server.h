#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>

#include "Total.h"
#include "Database.h"

class Server: public QTcpServer{
  Q_OBJECT

  public:
    Server() = delete;
    Server(uint16_t port, Database* database);

  public slots:
    void incomingConnection(qintptr socketDescriptor);
    void readData();

  private:
    void sendToClient(QTcpSocket* socketClient, const QString& string);

    Total* m_commander;

    QTcpSocket* m_socket;
    QVector<QTcpSocket*> m_clientSockets;
    QByteArray m_data;

    quint16 m_packageSize;
};

#endif // SERVER_H
