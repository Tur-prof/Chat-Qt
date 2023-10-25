#include "RegistrationForm.h"
#include "ui_RegistrationForm.h"

#include <QMessageBox>
#include "SHA_1.h"
#include "Parsing.h"


RegistrationForm::RegistrationForm(QWidget* parent) :
  QDialog(parent), ui(new Ui::RegistrationForm)
{
  ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
  delete ui;
}

void RegistrationForm::setClient(Client* client)
{
  client_ = client;
}

void RegistrationForm::handleServerResponse_IsUserRegistered()
{
  auto isUserRegistered = client_->getResponse();
  disconnectNetworkSignal_IsUserRegistered();

  if (isUserRegistered == "true"){
    QString message = "User already exists. Try to log in.";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  connectNetworkSignal_IsUserAdded();

  auto hashPassword = sha_1::hash(password_.toStdString());
  connectNetworkSignal_IsUserAdded();
  client_->requestAddUser(login_, QString::fromStdString(hashPassword));
}

void RegistrationForm::handleServerResponse_IsUserAdded()
{
  auto isUserAdded = client_->getResponse();
  disconnectNetworkSignal_IsUserAdded();

  if (isUserAdded == "true"){
    auto login = ui->loginEdit->text().toStdString();
    ui->loginEdit->clear();
    ui->passwordEdit->clear();
    ui->passwordConfirmEdit->clear();

    emit toChat(login);
  }
  else{
    QString message = "User is not added - something wrong";
    QMessageBox::critical(this, "Error", message);
    return;
  }
}

void RegistrationForm::on_loginButton_clicked()
{
  ui->loginEdit->clear();
  ui->passwordEdit->clear();
  ui->passwordConfirmEdit->clear();

  emit toLogin();
}

void RegistrationForm::on_buttonBox_accepted()
{
  login_.clear();
  password_.clear();

  auto login = ui->loginEdit->text().toStdString();
  //Check Login input
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

  auto confirmPassword = ui->passwordConfirmEdit->text().toStdString();
  if (confirmPassword.empty()){
    QString message = "Please type Confirm Password";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (password != confirmPassword){
    QString message = "Password doesn't match Confirm Password";
    QMessageBox::critical(this, "Error", "Password doesn't match Confirm Password");
    return;
  }

  login_ = QString::fromStdString(login);
  password_ = QString::fromStdString(password);

  checkIsUserRegistered(login_);
}

void RegistrationForm::on_buttonBox_rejected()
{
  emit close();
}

void RegistrationForm::connectNetworkSignal_IsUserRegistered()
{
  connect(client_, &Client::dataReceived,
          this, &RegistrationForm::handleServerResponse_IsUserRegistered);
}

void RegistrationForm::disconnectNetworkSignal_IsUserRegistered()
{
  disconnect(client_, &Client::dataReceived,
              this, &RegistrationForm::handleServerResponse_IsUserRegistered);
}

void RegistrationForm::connectNetworkSignal_IsUserAdded()
{
  connect(client_, &Client::dataReceived,
          this, &RegistrationForm::handleServerResponse_IsUserAdded);
}

void RegistrationForm::disconnectNetworkSignal_IsUserAdded()
{
  disconnect(client_, &Client::dataReceived,
              this, &RegistrationForm::handleServerResponse_IsUserAdded);
}

void RegistrationForm::checkIsUserRegistered(const QString& login)
{
  connectNetworkSignal_IsUserRegistered();
  client_->requestIsUserRegistered(login);
}
