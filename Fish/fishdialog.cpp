#include "fishdialog.h"
#include "ui_fishdialog.h"

FishDialog::FishDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FishDialog)
{
    ui->setupUi(this);
}

FishDialog::~FishDialog()
{
    delete ui;
}
