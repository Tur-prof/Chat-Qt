#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

public slots:
    void setAdminForm();
    void setMainWindow();
    void exit();

private:
    void setupSignals();

private:
    Ui::StartScreen *ui;
};

#endif // STARTSCREEN_H
