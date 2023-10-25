#include "AdminForm.h"
#include "ui_AdminForm.h"

#include <QMessageBox>

AdminForm::AdminForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminForm)
{
    ui->setupUi(this);
}

AdminForm::~AdminForm()
{
    delete ui;
}

void AdminForm::on_buttonBox_accepted()
{
    auto login = ui->loginEdit->text().toStdString();
    if (login.empty())
    {
        QString message = "Please type Login";
        QMessageBox::critical(this, "Error", message);
        return;
    }
    if (login != "Admin")
    {
        QString message = "Incorrect login";
        QMessageBox::critical(this, "Error", message);
        return;
    }

    auto password = ui->passwordEdit->text().toStdString();
    if (password != "Admin")
    {
        QString message = "Incorrect password";
        QMessageBox::critical(this, "Error", message);
        return;
    }
    emit accepted();
}

void AdminForm::on_buttonBox_rejected()
{
    emit rejected();
}

