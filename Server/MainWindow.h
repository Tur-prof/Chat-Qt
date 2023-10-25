#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Server.h"
#include "Database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_removeUserButton_clicked();
    void on_removeMessageButton_clicked();
    void on_banButton_clicked();
    void on_usersTableView_clicked(const QModelIndex &index);
    void on_messagesTableView_clicked(const QModelIndex &index);

    void updateViews();

private:
    Ui::MainWindow* ui;
    Server* m_server;
    Database* m_database;

    QSqlTableModel* modelUsers_;
    QSqlTableModel* modelMessages_;
    int activeUserRow_;
    int activeMessageRow_;
};
#endif // MAINWINDOW_H
