#ifndef AUTO_H
#define AUTO_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class Auto;
}

class Auto : public QDialog
{
    Q_OBJECT

public:
    explicit Auto(QWidget *parent = nullptr);
    ~Auto();
    void setDef();
     int getElect();
    int getBatt();
     int getProc();
     int getMode();
signals:
    void baseWindow();
    void butt2clicked();
private slots:
    void on_pushButton_clicked();
    void on_pushButton2_clicked();

private:
    Ui::Auto *ui;
};

#endif // AUTO_H

