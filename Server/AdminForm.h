#ifndef ADMINFORM_H
#define ADMINFORM_H

#include <QWidget>

namespace Ui {
class AdminForm;
}

class AdminForm : public QWidget
{
    Q_OBJECT

public:
    explicit AdminForm(QWidget *parent = nullptr);
    ~AdminForm();

signals:
    void loginAdmin();
    void accepted();
    void rejected();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AdminForm *ui;
};

#endif // ADMINFORM_H
