#include "LoginForm.h"
#include "ui_LoginForm.h"

#include <QMessageBox>
#include "SHA_1.h"
#include "Parsing.h"


LoginForm::LoginForm(QWidget* parent) :
  QDialog(parent), ui(new Ui::LoginForm)
{
  ui->setupUi(this);
}

LoginForm::~LoginForm()
{
  delete ui;
}

void LoginForm::setClient(Client* client)
{
  client_ = client;
}

void LoginForm::handleServerResponse_IsPasswordCorrect()
{
  auto isCorrectPassword = client_->getResponse();
  disconnectNetworkSignal_IsPasswordCorrect();

  if (isCorrectPassword == "true"){
    auto login = ui->loginEdit->text().toStdString();
    ui->loginEdit->clear();
    ui->passwordEdit->clear();

    emit toChat(login);
  }
  else{
    QMessageBox::critical(this, "Error", "Password incorrect");
    return;
  }
}

void LoginForm::on_registrationButton_clicked()
{
  ui->loginEdit->clear();
  ui->passwordEdit->clear();

  emit toRegistration();
}

void LoginForm::on_buttonBox_accepted()
{
  auto login = ui->loginEdit->text().toStdString();
  if (login.empty()){
    QString message = "Please type Login";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (Parsing::isContainsForbidenChars(login)){
    QString message = "Login contains forbidden symbols. Allowed: 'a'-'z', 'A'-'Z', '0'-'9')";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (Parsing::isStartsByNumber(login)){
    QString message = "Login starts by number";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (Parsing::isTooShort(login)){
    QString message = "Login too short. Min length: " +
                      QString::number(Parsing::getMinLength()) +
                      " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  auto password = ui->passwordEdit->text().toStdString();
  //Check Password input
  if (password.empty()){
    QString message = "Please type Password";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (Parsing::isTooShort(password)){
    QString message = "Password too short. Min length: " +
                      QString::number(Parsing::getMinLength()) +
                      " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }


  //Request verification of the login-password from the server
  auto hashPassword = sha_1::hash(password);
  connectNetworkSignal_IsPasswordCorrect();
  client_->requestIsPasswordCorrect(QString::fromStdString(login),
                                    QString::fromStdString(hashPassword));
}



void LoginForm::on_buttonBox_rejected()
{
  //Close application
  emit close();
}



//private======================================================================
void LoginForm::connectNetworkSignal_IsPasswordCorrect()
{
  connect(client_, &Client::dataReceived,
          this, &LoginForm::handleServerResponse_IsPasswordCorrect);
}



void LoginForm::disconnectNetworkSignal_IsPasswordCorrect()
{
  disconnect(client_, &Client::dataReceived,
              this, &LoginForm::handleServerResponse_IsPasswordCorrect);
}
