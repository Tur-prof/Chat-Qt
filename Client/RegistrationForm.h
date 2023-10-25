#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QDialog>

#include "Client.h"

namespace Ui {
  class RegistrationForm;
}

class RegistrationForm : public QDialog{
  Q_OBJECT

  public:
    explicit RegistrationForm(QWidget* parent = nullptr);
    ~RegistrationForm();

    void setClient(Client* client);

  public slots:
    void handleServerResponse_IsUserRegistered();
    void handleServerResponse_IsUserAdded();

  signals:
    void toLogin();
    void toChat(const std::string& userLogin);
    void close();

  private slots:
    void on_loginButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

  private:
    void connectNetworkSignal_IsUserRegistered();
    void disconnectNetworkSignal_IsUserRegistered();

    void connectNetworkSignal_IsUserAdded();
    void disconnectNetworkSignal_IsUserAdded();

    void checkIsUserRegistered(const QString& login);


    Ui::RegistrationForm* ui;
    Client* client_;

    QString login_;
    QString password_;
};

#endif // REGISTRATIONFORM_H
