#include "auto.h"
#include "ui_auto.h"
#include "mainwindow.h"
#include <QWidget>

Auto::Auto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Auto)
{
    ui->setupUi(this);
}

Auto::~Auto()
{
    delete ui;
}

void Auto::on_pushButton_clicked()
{
    this->close();
    emit baseWindow();
}


void Auto::on_pushButton2_clicked()
{
    this->close();
    emit butt2clicked();


}

void Auto::setDef()
{
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(1);
    ui->comboBox_4->setCurrentIndex(2);
    ui->groupBox_5->setChecked(false);
    ui->spinBox->setSpecialValueText("0");
}

int Auto::getElect()
{
    return ui->comboBox_4->currentIndex();
}

int Auto::getBatt()
{
    return ui->comboBox_3->currentIndex();
}

int Auto::getMode()
{
    if (ui->groupBox_5->isChecked())
           return ui->comboBox_2->currentIndex();
}

int Auto::getProc()
{
    if (ui->groupBox_5->isChecked())
        return ui->spinBox->value();
    else return -2;
}
