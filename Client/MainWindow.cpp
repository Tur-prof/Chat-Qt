#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>

MainWindow::MainWindow(QWidget* parent) :
  QWidget(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout,
          this, &MainWindow::updateMessages);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setClient(Client* client)
{
  client_ = client;
}

void MainWindow::connectNetworkSignal_UpdateMessage()
{
  timer_->start(50);
}

void MainWindow::setUserLogin(const std::string& userLogin)
{
  userLogin_ = userLogin;
}

void MainWindow::handleServerResponse_GetUsersList()
{
  auto response = client_->getResponse();
  disconnectNetworkSignal_GetUserList();
  QStringList userList = response.split("|", Qt::KeepEmptyParts);

  QDialog listUsersDialog(this);
  listUsersDialog.setModal(true);
  auto layout = new QVBoxLayout();
  listUsersDialog.setLayout(layout);
  auto userListWidget = new QListWidget(&listUsersDialog);
  layout->addWidget(userListWidget);
  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                        QDialogButtonBox::Cancel,
                                        &listUsersDialog);
  layout->addWidget(buttonBox);
  connect(buttonBox, &QDialogButtonBox::accepted,
          &listUsersDialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected,
          &listUsersDialog, &QDialog::reject);

  for (auto user : userList){
    userListWidget->addItem(user);
  }
  userListWidget->setCurrentRow(0);

  auto result = listUsersDialog.exec();
  if (result == QDialog::Accepted && userListWidget->currentItem()){
    auto receiver = userListWidget->currentItem()->text();
    auto message = ui->messageLineEdit->text();
    client_->sendPrivateMessage(QString::fromStdString(userLogin_),
                                receiver,
                                message);
  }
  ui->messageLineEdit->clear();
}

void MainWindow::handleServerResponse_GetCommonMessages()
{
  commonMessages_ = client_->getResponse();
  disconnectNetworkSignal_GetCommonMessages();
  connectNetworkSignal_GetPrivateMessages();
  client_->requestPrivateMessage(QString::fromStdString(userLogin_));
}

void MainWindow::handleServerResponse_GetPrivateMessages()
{
  privateMessages_ = client_->getResponse();
  disconnectNetworkSignal_GetPrivateMessages();
  displayMessages();
}

void MainWindow::on_publicMessageSendButton_clicked()
{
  auto message = ui->messageLineEdit->text().toStdString();
  if (message.empty()){
    QMessageBox::critical(this, "Error", "Please type message");
    return;
  }

  client_->sendCommonMessage(QString::fromStdString(userLogin_),
                             QString::fromStdString(message));
  ui->messageLineEdit->clear();
}

void MainWindow::on_privateMessageSendButton_clicked()
{
  auto message = ui->messageLineEdit->text().toStdString();
  if (message.empty()){
    QMessageBox::critical(this, "Error", "Please type message");
    return;
  }

  connectNetworkSignal_GetUserList();
  client_->requestUserLogins();
}

void MainWindow::on_exitButton_clicked()
{
  disconnectNetworkSignal_UpdateMessage();
  disconnectNetworkSignal_GetCommonMessages();
  disconnectNetworkSignal_GetPrivateMessages();

  ui->messageLineEdit->clear();

  emit toLogin();
}

void MainWindow::on_deleteAccountButton_clicked()
{
  disconnectNetworkSignal_UpdateMessage();
  disconnectNetworkSignal_GetCommonMessages();
  disconnectNetworkSignal_GetPrivateMessages();

  ui->messageLineEdit->clear();

  client_->requestRemoveUser(QString::fromStdString(userLogin_));

  emit toLogin();
}

void MainWindow::connectNetworkSignal_GetUserList()
{
  disconnectNetworkSignal_UpdateMessage();
  connect(client_, &Client::dataReceived,
          this, &MainWindow::handleServerResponse_GetUsersList);
}

void MainWindow::disconnectNetworkSignal_GetUserList()
{
  disconnect(client_, &Client::dataReceived,
            this, &MainWindow::handleServerResponse_GetUsersList);
  connectNetworkSignal_UpdateMessage();
}

void MainWindow::disconnectNetworkSignal_UpdateMessage()
{
  timer_->stop();
}

void MainWindow::connectNetworkSignal_GetCommonMessages()
{
  connect(client_, &Client::dataReceived,
          this, &MainWindow::handleServerResponse_GetCommonMessages);
}

void MainWindow::disconnectNetworkSignal_GetCommonMessages()
{
  disconnect(client_, &Client::dataReceived,
              this, &MainWindow::handleServerResponse_GetCommonMessages);
}

void MainWindow::connectNetworkSignal_GetPrivateMessages()
{
  connect(client_, &Client::dataReceived,
          this, &MainWindow::handleServerResponse_GetPrivateMessages);
}

void MainWindow::disconnectNetworkSignal_GetPrivateMessages()
{
  disconnect(client_, &Client::dataReceived,
            this, &MainWindow::handleServerResponse_GetPrivateMessages);
}

void MainWindow::updateMessages()
{
  connectNetworkSignal_GetCommonMessages();
  client_->requestCommonMessage();
}

void MainWindow::displayMessages()
{
  if (ui->publicMessagesTextBrowser->toPlainText() != commonMessages_){
    ui->publicMessagesTextBrowser->setText(commonMessages_);
  }

  if (ui->privateMessagesTextBrowser->toPlainText() != privateMessages_){
    ui->privateMessagesTextBrowser->setText(privateMessages_);
  }

  commonMessages_.clear();
  privateMessages_.clear();
}
