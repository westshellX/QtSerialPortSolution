#ifndef FISHDIALOG_H
#define FISHDIALOG_H

#include <QDialog>

namespace Ui {
class FishDialog;
}

class FishDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FishDialog(QWidget *parent = nullptr);
    ~FishDialog();

private:
    Ui::FishDialog *ui;
};

#endif // FISHDIALOG_H
