#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTimer>

#include "Client.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QWidget{
  Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void setClient(Client* client);
    void connectNetworkSignal_UpdateMessage();

  signals:
    void toLogin();

  public slots:
    void setUserLogin(const std::string& userLogin);

    void handleServerResponse_GetUsersList();
    void handleServerResponse_GetCommonMessages();
    void handleServerResponse_GetPrivateMessages();

  private slots:
    void on_publicMessageSendButton_clicked();
    void on_privateMessageSendButton_clicked();
    void on_exitButton_clicked();
    void on_deleteAccountButton_clicked();

  private:
    void connectNetworkSignal_GetUserList();
    void disconnectNetworkSignal_GetUserList();

    void disconnectNetworkSignal_UpdateMessage();

    void connectNetworkSignal_GetCommonMessages();
    void disconnectNetworkSignal_GetCommonMessages();

    void connectNetworkSignal_GetPrivateMessages();
    void disconnectNetworkSignal_GetPrivateMessages();

    void updateMessages();

    void displayMessages();

    Ui::MainWindow* ui;
    std::string userLogin_;
    Client* client_;
    QTimer* timer_;
    QString commonMessages_;
    QString privateMessages_;
};

#endif // MAINWINDOW_H
