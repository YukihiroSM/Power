#include "informativewindow.h"
#include "ui_informativewindow.h"
#include <mainwindow.h>
InformativeWindow::InformativeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformativeWindow)
{
    ui->setupUi(this);
}

InformativeWindow::~InformativeWindow()
{
    delete ui;
}

void InformativeWindow::on_pushButton_clicked()
{
    this->close();
    emit baseWindow();
}
