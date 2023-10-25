#include "Server.h"

#include "Total.h"

Server::Server(uint16_t port, Database* database)
{
  this->listen(QHostAddress::Any, port);
  m_packageSize = 0;
  m_socket = new QTcpSocket;
  m_commander = new Total(database);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
  m_socket->setSocketDescriptor(socketDescriptor);
  connect(m_socket, &QTcpSocket::readyRead, this, &Server::readData);
  connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);
  m_clientSockets.push_back(m_socket);
}

void Server::readData()
{
  m_socket = (QTcpSocket*) sender();
  QDataStream input(m_socket);
  input.setVersion(QDataStream::Qt_5_6);
  if (input.status() == QDataStream::Ok){
    for(;;){
      if (m_packageSize == 0){
        if (m_socket->bytesAvailable() < 2){
          break;
        }
        input >> m_packageSize;
      }
      if (m_socket->bytesAvailable() < m_packageSize){
        break;
      }
      QString commandArgs;
      int commandType;
      input >> commandType >> commandArgs;

      auto response = m_commander->commander(commandType, commandArgs);

      sendToClient(m_socket, response);

      m_packageSize = 0;
      break;
    }
  }
}

void Server::sendToClient(QTcpSocket* socketClient, const QString& string)
{
  m_data.clear();
  QDataStream output(&m_data, QIODevice::WriteOnly);
  output.setVersion(QDataStream::Qt_5_6);

  output << quint16(0) << string;
  output.device()->seek(0);

  output << quint16(m_data.size() - sizeof(quint16));
  socketClient->write(m_data);
}
