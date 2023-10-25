#include "StartScreen.h"
#include "ui_StartScreen.h"

#include <QtDebug>
#include <QMessageBox>
#include "SHA_1.h"

namespace{
  enum{
    LOGIN_WINDOW = 0,
    REGISTRATION_WINDOW,
    CHAT_WINDOW
  };
}

StartScreen::StartScreen(QWidget* parent):
  QDialog(parent), ui(new Ui::StartScreen)
{
  ui->setupUi(this);

  client_ = new Client("127.0.0.1", 7777);

  ui->loginWidget->setClient(client_);
  ui->registrWidget->setClient(client_);
  ui->mainWindow->setClient(client_);

  setLoginWindow();

  setupSignals();
}

StartScreen::~StartScreen()
{
  delete ui;
  qApp->exit(0);
}

void StartScreen::setLoginWindow()
{
  ui->stackedWidget->setCurrentIndex(LOGIN_WINDOW);
}

void StartScreen::setRegistrationWindow()
{
  ui->stackedWidget->setCurrentIndex(REGISTRATION_WINDOW);
}

void StartScreen::setChatWindow()
{
  ui->stackedWidget->setCurrentIndex(CHAT_WINDOW);
  ui->mainWindow->connectNetworkSignal_UpdateMessage();
}

void StartScreen::exit()
{
  this->close();
}

void StartScreen::handleNetworkFailed()
{
  QString message = "Server connection failed. Contact the server administrator or try to connect later.";
  QMessageBox::critical(this, "Error", message);
  this->close();
}

void StartScreen::setupSignals()
{ 
  connect(client_, &Client::failConnection,
        this, &StartScreen::handleNetworkFailed);

  connect(ui->loginWidget, &LoginForm::close,
        this, &StartScreen::exit);

  connect(ui->registrWidget, &RegistrationForm::close,
        this, &StartScreen::exit);

  connect(ui->loginWidget, &LoginForm::toRegistration,
          this, &StartScreen::setRegistrationWindow);

  connect(ui->loginWidget, &LoginForm::toChat,
          this, &StartScreen::setChatWindow);

  connect(ui->loginWidget, &LoginForm::toChat,
          ui->mainWindow, &MainWindow::setUserLogin);

  connect(ui->registrWidget, &RegistrationForm::toLogin,
          this, &StartScreen::setLoginWindow);
  connect(ui->registrWidget, &RegistrationForm::toChat,
          this, &StartScreen::setChatWindow);

  connect(ui->registrWidget, &RegistrationForm::toChat,
          ui->mainWindow, &MainWindow::setUserLogin);

  connect(ui->mainWindow, &MainWindow::toLogin,
          this, &StartScreen::setLoginWindow);
}
