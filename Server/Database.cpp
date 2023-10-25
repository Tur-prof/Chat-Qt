#include "Database.h"

#include <QtDebug>
#include <QDateTime>


namespace {
  const std::string ALL = "ALL";
}

Database::Database(): m_database(QSqlDatabase::addDatabase("QSQLITE"))
{
  m_database.setDatabaseName("./chatDB.db");
  if (m_database.open()){
    qDebug() << "Database is opened";
  }
  else{
    qDebug() << "Database is not opened";
  }

  m_query = new QSqlQuery(m_database);
  m_query->exec("CREATE TABLE Users(Login TEXT, PasswordHash TEXT, Ban BOOL, Online BOOL);");
  m_query->exec("CREATE TABLE Messages(Sender TEXT, Receiver TEXT, Message TEXT, Date TEXT);");
}



bool Database::isPasswordCorrect(const std::string& login, const std::string& passwordHash) const
{
  if (login.empty() || passwordHash.empty()){
    return false;
  }
  if (!isUserRegistered(login)){
    return false;
  }

  auto request = "Select PasswordHash from Users WHERE Login = '" +
                  QString::fromStdString(login) + "';";
  m_query->exec(request);

  QString databasePasswordHash;
  while (m_query->next()) {
    databasePasswordHash = m_query->value(0).toString();
  }

  if (databasePasswordHash != QString::fromStdString(passwordHash) ){
    return false;
  }
  request.clear();
  request = "UPDATE Users SET Online = true WHERE Login = '" +
                 QString::fromStdString(login) + "';";
  m_query->exec(request);

  return true;
}

bool Database::isUserRegistered(const std::string& login) const
{
  if (login.empty()){
    return false;
  }

  auto request = "Select * from Users WHERE Login = '" +
                  QString::fromStdString(login) + "';";
  m_query->exec(request);
  //User not found
  if (!m_query->next()){
    return false;
  }

  return true;
}

bool Database::addUser(const std::string& login, const std::string& passwordHash)
{
  if (login.empty() || passwordHash.empty()){
    return false;
  }

  if (isUserRegistered(login)) {
    return false;
  }

  auto request = "INSERT INTO Users(Login, PasswordHash, Ban, Online) VALUES('" +
                  QString::fromStdString(login) + "', '" +
                  QString::fromStdString(passwordHash) + "', false, true);";
  m_query->exec(request);

  emit updateDatabase();

  return true;
}

bool Database::addPublicMessage(const std::string& sender, const std::string& message)
{
  if (message.empty()){
    return false;
  }

  if (!isUserRegistered(sender)) {
    return false;
  }

  if (getIsBanned(sender)){
    return false;
  }

  auto request = "INSERT INTO Messages(Sender, Receiver, Message, Date) VALUES('" +
                  QString::fromStdString(sender) + "', '" +
                  QString::fromStdString(ALL) + "', '" +
                  QString::fromStdString(message) + "', '" +
                  QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "');";
  m_query->exec(request);

  emit updateDatabase();

  return true;
}

bool Database::addPrivateMessage(const std::string& sender, const std::string& receiver, const std::string& message)
{
  if (message.empty()){
    return false;
  }

  if (!isUserRegistered(sender)) {
    return false;
  }

  if (!isUserRegistered(receiver)) {
    return false;
  }

  if (getIsBanned(sender)){
    return false;
  }

  auto request = "INSERT INTO Messages(Sender, Receiver, Message, Date) VALUES('" +
                  QString::fromStdString(sender) + "', '" +
                  QString::fromStdString(receiver) + "', '" +
                  QString::fromStdString(message) + "', '" +
                  QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "');";
  m_query->exec(request);

  emit updateDatabase();
  return true;
}

std::string Database::getPublicMessages() const
{
  auto request = "Select * from Messages WHERE Receiver = '" +
                  QString::fromStdString(ALL) + "';";
  m_query->exec(request);

  std::string messages;
  while (m_query->next()) {
    auto sender = m_query->value(0).toString();
    auto receiver = m_query->value(1).toString();
    auto text = m_query->value(2).toString();
    auto date = m_query->value(3).toString();

    auto line = date + " <" + sender + ">: " + text + "\n";
    messages.append(line.toStdString());
  }
  return messages;
}

std::string Database::getPrivateMessages(const std::string& receiver) const
{
  auto request = "Select * from Messages WHERE Receiver = '" +
                  QString::fromStdString(receiver) + "' OR Sender = '" +
                  QString::fromStdString(receiver) + "' AND Receiver NOT IN ('" +
                  QString::fromStdString(ALL) + "');";

  m_query->exec(request);

  std::string messages;
  while (m_query->next()) {
    auto sender = m_query->value(0).toString();
    auto addressee = m_query->value(1).toString();
    auto text = m_query->value(2).toString();
    auto date = m_query->value(3).toString();

    auto line = date + " <" + sender + "> -> <" + addressee + ">: " + text + "\n";
    messages.append(line.toStdString());
  }

  return messages;
}

std::list<std::string> Database::getLogins() const
{
  m_query->exec("Select * from Users;");

  std::list<std::string> logins;
  while (m_query->next()) {
    auto login = m_query->value(0).toString();
    logins.push_back(login.toStdString());
  }
  return logins;
}

bool Database::removeUser(const std::string& login)
{
  if (login.empty()){
    return false;
  }

  if (!isUserRegistered(login)) {
    return false;
  }

  auto request = "DELETE FROM Messages WHERE Receiver = '" +
                  QString::fromStdString(login) + "';";
  m_query->exec(request);

  request.clear();
  request = "DELETE FROM Users WHERE Login = '" +
            QString::fromStdString(login) + "';";
  m_query->exec(request);

  emit updateDatabase();

  return true;
}

bool Database::removeMessage(const std::string& message)
{
  if (message.empty()){
    return false;
  }

  auto request = "DELETE FROM Messages WHERE Message = '" +
                  QString::fromStdString(message) + "';";
  m_query->exec(request);

  emit updateDatabase();

  return true;
}

void Database::switchBanUser(const std::string& login)
{
  if(getIsBanned(login)){
    //Unban
    auto request = "UPDATE Users SET Ban = false WHERE Login = '" +
              QString::fromStdString(login) + "';";
    m_query->exec(request);
  }
  else{
    auto request = "UPDATE Users SET Ban = true WHERE Login = '" +
              QString::fromStdString(login) + "';";
    m_query->exec(request);
  }

  emit updateDatabase();
}

QSqlDatabase* Database::getDatabase()
{
  return &m_database;
}

bool Database::getIsBanned(const std::string& login) const
{
  if (login.empty()){
    return true;
  }
  if (!isUserRegistered(login)){
    return true;
  }

  auto request = "Select Ban from Users WHERE Login = '" +
                  QString::fromStdString(login) + "';";
  m_query->exec(request);

  bool isBanned = true;
  while (m_query->next()) {
    isBanned = m_query->value(0).toBool();
  }
  return isBanned;
}

bool Database::getOnlineStatus(const std::string& login) const
{
  if (login.empty()){
    return true;
  }
  if (!isUserRegistered(login)){
    return true;
  }

  auto request = "Select Online from Users WHERE Login = '" +
                 QString::fromStdString(login) + "';";
  m_query->exec(request);

  bool isOnline = true;
  while (m_query->next()) {
    isOnline = m_query->value(0).toBool();
  }
  return isOnline;
}
