#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include "Client.h"

namespace Ui {
  class LoginForm;
}

class LoginForm : public QDialog{
  Q_OBJECT

  public:
    explicit LoginForm(QWidget* parent = nullptr);
    ~LoginForm();

    void setClient(Client* client);

  signals:
    void toRegistration();
    void toChat(const std::string& userLogin);
    void close();

  public slots:
    void handleServerResponse_IsPasswordCorrect();

  private slots:
    void on_registrationButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

  private:
    void connectNetworkSignal_IsPasswordCorrect();
    void disconnectNetworkSignal_IsPasswordCorrect();

    Ui::LoginForm* ui;
    Client* client_;
};

#endif // LOGINFORM_H
