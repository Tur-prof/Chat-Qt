#include "StartScreen.h"
#include "ui_StartScreen.h"

StartScreen::StartScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    ui->loginWidget;
    ui->mainWindowWidget;

    setAdminForm();

    setupSignals();

}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setAdminForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setMainWindow()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::exit()
{
    this->close();
}

void StartScreen::setupSignals()
{
    connect(ui->loginWidget, &AdminForm::rejected,
            this, &StartScreen::exit);
    connect(ui->loginWidget, &AdminForm::accepted,
            this, &StartScreen::setMainWindow);
}
