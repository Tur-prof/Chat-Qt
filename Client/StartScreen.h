#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>

#include "Client.h"

namespace Ui {
  class StartScreen;
}

class StartScreen : public QDialog{
  Q_OBJECT

  public:
    explicit StartScreen(QWidget* parent = nullptr);
    ~StartScreen();

  public slots:
    void setLoginWindow();
    void setRegistrationWindow();
    void setChatWindow();
    void exit();

    void handleNetworkFailed();

  private:
    void setupSignals();

    Ui::StartScreen* ui;
    Client* client_;
};

#endif // STARTSCREEN_H
