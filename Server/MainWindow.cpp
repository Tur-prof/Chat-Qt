#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent), ui(new Ui::MainWindow),
  activeUserRow_(0), activeMessageRow_(0)
{
  ui->setupUi(this);

  m_database = new Database;

  modelUsers_ = new QSqlTableModel(this, *m_database->getDatabase());
  modelMessages_ = new QSqlTableModel(this, *m_database->getDatabase());
  modelUsers_->setTable("Users");
  modelMessages_->setTable("Messages");
  modelUsers_->select();
  modelMessages_->select();

  ui->usersTableView->setModel(modelUsers_);
  ui->messagesTableView->setModel(modelMessages_);

  const uint16_t port = 7777;
  m_server = new Server(port, m_database);

  connect(m_database, &Database::updateDatabase,
          this, &MainWindow::updateViews);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_removeUserButton_clicked()
{
  auto idCell = ui->usersTableView->model()->index(activeUserRow_, 0);
  auto loginToDelete = ui->usersTableView->model()->data(idCell).toString();

  //Create screen to confirm user removing
  QDialog dialog(this);
  dialog.setModal(true);
  auto layout = new QVBoxLayout();
  dialog.setLayout(layout);

  auto label = new QLabel(&dialog);
  QString message = "Are sure to remove user: " + loginToDelete + " ?";
  label->setText(message);
  label->setAlignment(Qt::AlignHCenter);

  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes |
                                            QDialogButtonBox::No,
                                        &dialog);

  layout->setAlignment(buttonBox, Qt::AlignHCenter);

  auto splitter = new QSplitter(&dialog);
  splitter->setOrientation(Qt::Vertical);
  splitter->addWidget(label);
  splitter->addWidget(buttonBox);
  layout->addWidget(splitter);

  connect(buttonBox, &QDialogButtonBox::accepted,
          &dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected,
          &dialog, &QDialog::reject);

  auto result = dialog.exec();
  if (result == QDialog::Accepted){
    m_database->removeUser(loginToDelete.toStdString());
    activeUserRow_ = 0;
  }
}


void MainWindow::on_removeMessageButton_clicked()
{
  auto idCell = ui->messagesTableView->model()->index(activeMessageRow_, 2);
  auto messageText = ui->messagesTableView->model()->data(idCell).toString();

  //Create screen to confirm user removing
  QDialog dialog(this);
  dialog.setModal(true);
  auto layout = new QVBoxLayout();
  dialog.setLayout(layout);

  auto label = new QLabel(&dialog);
  QString message = "Are sure to remove message: '" + messageText + "' ?";
  label->setText(message);
  label->setAlignment(Qt::AlignHCenter);

  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes |
                                            QDialogButtonBox::No,
                                        &dialog);

  layout->setAlignment(buttonBox, Qt::AlignHCenter);

  auto splitter = new QSplitter(&dialog);
  splitter->setOrientation(Qt::Vertical);
  splitter->addWidget(label);
  splitter->addWidget(buttonBox);
  layout->addWidget(splitter);

  connect(buttonBox, &QDialogButtonBox::accepted,
          &dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected,
          &dialog, &QDialog::reject);

  auto result = dialog.exec();
  if (result == QDialog::Accepted){
    m_database->removeMessage(messageText.toStdString());
    activeMessageRow_ = 0;
  }
}

void MainWindow::on_banButton_clicked()
{
  auto idCell = ui->usersTableView->model()->index(activeUserRow_, 0);
  auto login = ui->usersTableView->model()->data(idCell).toString();
  m_database->switchBanUser(login.toStdString());
}


void MainWindow::on_usersTableView_clicked(const QModelIndex &index)
{
    activeUserRow_ = index.row();
}


void MainWindow::on_messagesTableView_clicked(const QModelIndex &index)
{
    activeMessageRow_ = index.row();
}

void MainWindow::updateViews()
{
    modelUsers_->select();
    modelMessages_->select();
}
