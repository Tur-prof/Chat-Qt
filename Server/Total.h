#ifndef TOTAL_H
#define TOTAL_H

#include <QString>
#include "Database.h"

class Total{
  public:
    Total() = delete;
    explicit Total(Database* database);

    QString commander(int commandType, const QString& args);

  private:
    QString isPasswordCorrect(const QString& args);
    QString isUserRegistered(const QString& args);
    QString addUser(const QString& args);
    QString addPublicMessage(const QString& args);
    QString addPrivateMessage(const QString& args);
    QString getPublicMessage();
    QString getPrivateMessage(const QString& args);
    QString getUserLogins();
    QString removeUser(const QString& args);

    Database* database_;
};

#endif // TOTAL_H
