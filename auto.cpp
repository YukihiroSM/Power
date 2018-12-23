#include "auto.h"
#include "ui_auto.h"

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
      int elect = ui->comboBox_2->currentIndex();
      int batt = ui->comboBox_3->currentIndex();
      if (ui->groupBox_5->isChecked())
      {
          int perc = ui->spinBox->value();
          int mode = ui->comboBox_4->currentIndex();
          this->close();
          emit SetAuto(elect, batt, perc, mode);

      }
      else {
          emit SetAuto(elect, batt, -2, 0);
      }
}

void Auto::setDef()
{
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(1);
    ui->comboBox_4->setCurrentIndex(2);
    ui->groupBox_5->setChecked(false);
    ui->spinBox->setSpecialValueText("0");
}
