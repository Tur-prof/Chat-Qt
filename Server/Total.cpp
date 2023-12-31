#include "Total.h"

namespace{
  //Command type
  enum Command{
    IS_PASSWORD_CORRECT,
    IS_USER_REGISTERED,
    ADD_USER,
    ADD_COMMON_MESSAGE,
    ADD_PRIVATE_MESSAGE,
    GET_COMMON_MESSAGE,
    GET_PRIVATE_MESSAGE,
    GET_USER_LOGINS,
    REMOVE_USER
  };
}

Total::Total(Database* database): database_(database)
{
}

QString Total::commander(int commandType, const QString& args)
{
  QString response;
  switch(commandType){
    case IS_PASSWORD_CORRECT:
      response = isPasswordCorrect(args);
      break;
    case IS_USER_REGISTERED:
      response = isUserRegistered(args);
      break;
    case ADD_USER:
      response = addUser(args);
      break;
    case ADD_COMMON_MESSAGE:
      response = addPublicMessage(args);
      break;
    case ADD_PRIVATE_MESSAGE:
      response = addPrivateMessage(args);
      break;
    case GET_COMMON_MESSAGE:
      response = getPublicMessage();
      break;
    case GET_PRIVATE_MESSAGE:
      response = getPrivateMessage(args);
      break;
    case GET_USER_LOGINS:
      response = getUserLogins();
      break;
    case REMOVE_USER:
      response = removeUser(args);
      break;
    default:
      break;
  }
  return response;
}

QString Total::isPasswordCorrect(const QString& args)
{
  //args: LOGIN|HASHPASSWORD
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  const std::string passwordHash = listArgs.at(1).toStdString();

  auto isOk = database_->isPasswordCorrect(login, passwordHash);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}

QString Total::isUserRegistered(const QString& args)
{
  //args: LOGIN|
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  auto isOk = database_->isUserRegistered(login);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}

QString Total::addUser(const QString& args)
{
  //args: LOGIN|HASHPASSWORD
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  const std::string passwordHash = listArgs.at(1).toStdString();

  auto isOk = database_->addUser(login, passwordHash);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}

QString Total::addPublicMessage(const QString& args)
{
  //args: SENDER|MESSAGE_TEXT
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string sender = listArgs.at(0).toStdString();
  const std::string message = listArgs.at(1).toStdString();

  auto isOk = database_->addPublicMessage(sender, message);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}

QString Total::addPrivateMessage(const QString& args)
{
  //args: SENDER|RECEIVER|MESSAGE_TEXT
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string sender = listArgs.at(0).toStdString();
  const std::string receiver = listArgs.at(1).toStdString();
  const std::string message = listArgs.at(2).toStdString();

  auto isOk = database_->addPrivateMessage(sender, receiver, message);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}

QString Total::getPublicMessage()
{
  auto messages = database_->getPublicMessages();
  auto response = QString::fromStdString(messages);
  return response;
}

QString Total::getPrivateMessage(const QString& args)
{
  //args: RECEIVER|
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string receiver = listArgs.at(0).toStdString();

  auto messages = database_->getPrivateMessages(receiver);
  auto response = QString::fromStdString(messages);
  return response;
}

QString Total::getUserLogins()
{
  auto users = database_->getLogins();
  QString response;
  for (const auto& user : users){
    response += QString::fromStdString(user) + "|";
  }
  return response;
}

QString Total::removeUser(const QString& args)
{
  //args: LOGIN|
  QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();

  auto isOk = database_->removeUser(login);
  QString response = "false";
  if (isOk){
    response = "true";
  }

  return response;
}
